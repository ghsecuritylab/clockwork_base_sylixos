/******************************************************************************
 *
 * Module Name: dtio.c - File I/O support for data table compiler
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2011, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/

#define __DTIO_C__

#include "aslcompiler.h"
#include "dtcompiler.h"

#define _COMPONENT          DT_COMPILER
        ACPI_MODULE_NAME    ("dtio")


/* Local prototypes */

static char *
DtTrim (
    char                    *String);

static void
DtLinkField (
    DT_FIELD                *Field);

static void
DtMergeField (
    char                    *Value);

static ACPI_STATUS
DtParseLine (
    char                    *LineBuffer,
    UINT32                  Line,
    UINT32                  Offset);

UINT32
DtGetNextLine (
    FILE                    *Handle);

static void
DtWriteBinary (
    DT_SUBTABLE             *Subtable,
    void                    *Context,
    void                    *ReturnValue);

static void
DtDumpBuffer (
    UINT32                  FileId,
    UINT8                   *Buffer,
    UINT32                  Offset,
    UINT32                  Length);


/* States for DtGetNextLine */

#define DT_NORMAL_TEXT              0
#define DT_START_QUOTED_STRING      1
#define DT_START_COMMENT            2
#define DT_SLASH_ASTERISK_COMMENT   3
#define DT_SLASH_SLASH_COMMENT      4
#define DT_END_COMMENT              5

static UINT32  Gbl_NextLineOffset;


/******************************************************************************
 *
 * FUNCTION:    DtTrim
 *
 * PARAMETERS:  String              - Current source code line to trim
 *
 * RETURN:      Trimmed line. Must be freed by caller.
 *
 * DESCRIPTION: Trim left and right spaces
 *
 *****************************************************************************/

static char *
DtTrim (
    char                    *String)
{
    char                    *Start;
    char                    *End;
    char                    *ReturnString;
    ACPI_SIZE               Length;


    /* Skip lines that start with a space */

    if (!ACPI_STRCMP (String, " "))
    {
        ReturnString = UtLocalCalloc (1);
        return (ReturnString);
    }

    /* Setup pointers to start and end of input string */

    Start = String;
    End = String + ACPI_STRLEN (String) - 1;

    /* Find first non-whitespace character */

    while ((Start <= End) && ((*Start == ' ') || (*Start == '\t')))
    {
        Start++;
    }

    /* Find last non-space character */

    while (End >= Start)
    {
        if (*End == '\r' || *End == '\n')
        {
            End--;
            continue;
        }

        if (*End != ' ')
        {
            break;
        }

        End--;
    }

    /* Remove any quotes around the string */

    if (*Start == '\"')
    {
        Start++;
    }
    if (*End == '\"')
    {
        End--;
    }

    /* Create the trimmed return string */

    Length = ACPI_PTR_DIFF (End, Start) + 1;
    ReturnString = UtLocalCalloc (Length + 1);
    if (ACPI_STRLEN (Start))
    {
        ACPI_STRNCPY (ReturnString, Start, Length);
    }

    ReturnString[Length] = 0;
    return (ReturnString);
}


/******************************************************************************
 *
 * FUNCTION:    DtLinkField
 *
 * PARAMETERS:  Field               - New field object to link
 *
 * RETURN:      None
 *
 * DESCRIPTION: Link one field name and value to the list
 *
 *****************************************************************************/

static void
DtLinkField (
    DT_FIELD                *Field)
{
    DT_FIELD                *Prev;
    DT_FIELD                *Next;


    Prev = Next = Gbl_FieldList;

    while (Next)
    {
        Prev = Next;
        Next = Next->Next;
    }

    if (Prev)
    {
        Prev->Next = Field;
    }
    else
    {
        Gbl_FieldList = Field;
    }
}


/******************************************************************************
 *
 * FUNCTION:    DtMergeField
 *
 * PARAMETERS:  Value               - Merge this line into previous one
 *
 * RETURN:      None
 *
 * DESCRIPTION: Merge a field value to the previous one,
 *              probably for a multi-line buffer definition.
 *
 *****************************************************************************/

static void
DtMergeField (
    char                    *Value)
{
    DT_FIELD                *Prev;
    DT_FIELD                *Next;
    char                    *NewValue;
    UINT32                  PrevLength;
    UINT32                  ThisLength;


    Prev = Next = Gbl_FieldList;

    while (Next)
    {
        Prev = Next;
        Next = Next->Next;
    }

    if (Prev)
    {
        PrevLength = ACPI_STRLEN (Prev->Value);
        ThisLength = ACPI_STRLEN (Value);

        /* Add two for: separator + NULL terminator */

        NewValue = UtLocalCalloc (PrevLength + ThisLength + 2);
        ACPI_STRNCPY (NewValue, Prev->Value, PrevLength);
        NewValue[PrevLength] = ' ';

        ACPI_STRNCPY ((NewValue + PrevLength + 1), Value, ThisLength);
        ACPI_FREE (Prev->Value);
        Prev->Value = NewValue;
    }
}


/******************************************************************************
 *
 * FUNCTION:    DtParseLine
 *
 * PARAMETERS:  LineBuffer          - Current source code line
 *              Line                - Current line number in the source
 *              Offset              - Current byte offset of the line
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Parse one source line
 *
 *****************************************************************************/

static ACPI_STATUS
DtParseLine (
    char                    *LineBuffer,
    UINT32                  Line,
    UINT32                  Offset)
{
    char                    *Start;
    char                    *End;
    char                    *TmpName;
    char                    *TmpValue;
    char                    *Name;
    char                    *Value;
    char                    *Colon;
    UINT32                  Length;
    DT_FIELD                *Field;
    UINT32                  Column;
    UINT32                  NameColumn;


    if (!LineBuffer)
    {
        return (AE_OK);
    }

    /* All lines after "Raw Table Data" are ingored */

    if (strstr (LineBuffer, ACPI_RAW_TABLE_DATA_HEADER))
    {
        return (AE_NOT_FOUND);
    }

    Colon = strchr (LineBuffer, ':');
    if (!Colon)
    {
        return (AE_OK);
    }

    Start = LineBuffer;
    End = Colon;

    while (Start < Colon)
    {
        if (*Start == ' ')
        {
            Start++;
            continue;
        }

        /* Found left bracket, go to the right bracket */

        if (*Start == '[')
        {
            while (Start < Colon && *Start != ']')
            {
                Start++;
            }

            if (Start == Colon)
            {
                break;
            }

            Start++;
            continue;
        }

        break;
    }

    /*
     * There are two column values. One for the field name,
     * and one for the field value.
     */
    Column = ACPI_PTR_DIFF (Colon, LineBuffer) + 3;
    NameColumn = ACPI_PTR_DIFF (Start, LineBuffer) + 1;

    Length = ACPI_PTR_DIFF (End, Start);

    TmpName = UtLocalCalloc (Length + 1);
    ACPI_STRNCPY (TmpName, Start, Length);
    Name = DtTrim (TmpName);
    ACPI_FREE (TmpName);

    Start = End = (Colon + 1);

    while (*End)
    {
        /* Found left quotation, go to the right quotation and break */

        if (*End == '"')
        {
            End++;
            while (*End && (*End != '"'))
            {
                End++;
            }

            End++;
            break;
        }

        /*
         * Special "comment" fields at line end, ignore them.
         * Note: normal slash-slash and slash-asterisk comments are
         * stripped already by the DtGetNextLine parser.
         *
         * TBD: Perhaps DtGetNextLine should parse the following type
         * of comments also.
         */
        if (*End == '(' ||
            *End == '<')
        {
            break;
        }

        End++;
    }

    Length = ACPI_PTR_DIFF (End, Start);
    TmpValue = UtLocalCalloc (Length + 1);

    ACPI_STRNCPY (TmpValue, Start, Length);
    Value = DtTrim (TmpValue);
    ACPI_FREE (TmpValue);

    if (ACPI_STRLEN (Name) && Value)
    {
        Field = UtLocalCalloc (sizeof (DT_FIELD));
        Field->Name = Name;
        Field->Value = Value;
        Field->Line = Line;
        Field->ByteOffset = Offset;
        Field->NameColumn = NameColumn;
        Field->Column = Column;

        DtLinkField (Field);
    }
    else if (!ACPI_STRLEN (Name))
    {
        /* Handle multi-line buffers (length > 16) */

        DtMergeField (Value);
    }
    else
    {
        ACPI_FREE (Name);
        ACPI_FREE (Value);
    }

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    DtGetNextLine
 *
 * PARAMETERS:  Handle              - Open file handle for the source file
 *
 * RETURN:      Filled line buffer and offset of start-of-line (zero on EOF)
 *
 * DESCRIPTION: Get the next valid source line. Removes all comments.
 *              Ignores empty lines.
 *
 * Handles both slash-asterisk and slash-slash comments.
 * Also, quoted strings, but no escapes within.
 *
 * Line is returned in Gbl_CurrentLineBuffer.
 * Line number in original file is returned in Gbl_CurrentLineNumber.
 *
 *****************************************************************************/

UINT32
DtGetNextLine (
    FILE                    *Handle)
{
    UINT32                  State = DT_NORMAL_TEXT;
    UINT32                  CurrentLineOffset;
    UINT32                  i;
    char                    c;


    for (i = 0; i < ASL_LINE_BUFFER_SIZE;)
    {
        c = (char) getc (Handle);
        if (c == EOF)
        {
            switch (State)
            {
            case DT_START_QUOTED_STRING:
            case DT_SLASH_ASTERISK_COMMENT:
            case DT_SLASH_SLASH_COMMENT:

                AcpiOsPrintf ("**** EOF within comment/string %u\n", State);
                break;

            default:
                break;
            }

            return (0);
        }

        switch (State)
        {
        case DT_NORMAL_TEXT:

            /* Normal text, insert char into line buffer */

            Gbl_CurrentLineBuffer[i] = c;
            switch (c)
            {
            case '/':
                State = DT_START_COMMENT;
                break;

            case '"':
                State = DT_START_QUOTED_STRING;
                i++;
                break;

            case '\n':
                CurrentLineOffset = Gbl_NextLineOffset;
                Gbl_NextLineOffset = (UINT32) ftell (Handle);
                Gbl_CurrentLineNumber++;

                /* Exit if line is complete. Ignore blank lines */

                if (i != 0)
                {
                    Gbl_CurrentLineBuffer[i+1] = 0; /* Terminate line */
                    return (CurrentLineOffset);
                }
                break;

            default:
                i++;
                break;
            }
            break;

        case DT_START_QUOTED_STRING:

            /* Insert raw chars until end of quoted string */

            Gbl_CurrentLineBuffer[i] = c;
            i++;

            if (c == '"')
            {
                State = DT_NORMAL_TEXT;
            }
            break;

        case DT_START_COMMENT:

            /* Open comment if this character is an asterisk or slash */

            switch (c)
            {
            case '*':
                State = DT_SLASH_ASTERISK_COMMENT;
                break;

            case '/':
                State = DT_SLASH_SLASH_COMMENT;
                break;

            default:    /* Not a comment */
                i++;    /* Save the preceeding slash */
                Gbl_CurrentLineBuffer[i] = c;
                i++;
                State = DT_NORMAL_TEXT;
                break;
            }
            break;

        case DT_SLASH_ASTERISK_COMMENT:

            /* Ignore chars until an asterisk-slash is found */

            switch (c)
            {
            case '\n':
                Gbl_NextLineOffset = (UINT32) ftell (Handle);
                Gbl_CurrentLineNumber++;
                break;

            case '*':
                State = DT_END_COMMENT;
                break;

            default:
                break;
            }
            break;

        case DT_SLASH_SLASH_COMMENT:

            /* Ignore chars until end-of-line */

            if (c == '\n')
            {
                /* We will exit via the NORMAL_TEXT path */

                ungetc (c, Handle);
                State = DT_NORMAL_TEXT;
            }
            break;

        case DT_END_COMMENT:

            /* End comment if this char is a slash */

            switch (c)
            {
            case '/':
                State = DT_NORMAL_TEXT;
                break;

            case '\n':
                CurrentLineOffset = Gbl_NextLineOffset;
                Gbl_NextLineOffset = (UINT32) ftell (Handle);
                Gbl_CurrentLineNumber++;
                break;

            case '*':
                /* Consume all adjacent asterisks */
                break;

            default:
                State = DT_SLASH_ASTERISK_COMMENT;
                break;
            }
            break;

        default:
            DtFatal (ASL_MSG_COMPILER_INTERNAL, NULL, "Unknown input state");
            return (0);
        }
    }

    printf ("ERROR - Input line is too long (max %u)\n", ASL_LINE_BUFFER_SIZE);
    return (0);
}


/******************************************************************************
 *
 * FUNCTION:    DtScanFile
 *
 * PARAMETERS:  Handle              - Open file handle for the source file
 *
 * RETURN:      Pointer to start of the constructed parse tree.
 *
 * DESCRIPTION: Scan source file, link all field names and values
 *              to the global parse tree: Gbl_FieldList
 *
 *****************************************************************************/

DT_FIELD *
DtScanFile (
    FILE                    *Handle)
{
    ACPI_STATUS             Status;
    UINT32                  Offset;


    ACPI_FUNCTION_NAME (DtScanFile);


    /* Get the file size */

    Gbl_InputByteCount = DtGetFileSize (Handle);

    Gbl_CurrentLineNumber = 0;
    Gbl_CurrentLineOffset = 0;
    Gbl_NextLineOffset = 0;

    /* Scan line-by-line */

    while ((Offset = DtGetNextLine (Handle)))
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_PARSE, "Line %2.2u/%4.4X - %s",
            Gbl_CurrentLineNumber, Offset, Gbl_CurrentLineBuffer));

        Status = DtParseLine (Gbl_CurrentLineBuffer, Gbl_CurrentLineNumber, Offset);
        if (Status == AE_NOT_FOUND)
        {
            break;
        }
    }

    return (Gbl_FieldList);
}


/*
 * Output functions
 */

/******************************************************************************
 *
 * FUNCTION:    DtWriteBinary
 *
 * PARAMETERS:  DT_WALK_CALLBACK
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write one subtable of a binary ACPI table
 *
 *****************************************************************************/

static void
DtWriteBinary (
    DT_SUBTABLE             *Subtable,
    void                    *Context,
    void                    *ReturnValue)
{

    FlWriteFile (ASL_FILE_AML_OUTPUT, Subtable->Buffer, Subtable->Length);
}


/******************************************************************************
 *
 * FUNCTION:    DtOutputBinary
 *
 * PARAMETERS:
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write entire binary ACPI table (result of compilation)
 *
 *****************************************************************************/

void
DtOutputBinary (
    DT_SUBTABLE             *RootTable)
{

    if (!RootTable)
    {
        return;
    }

    /* Walk the entire parse tree, emitting the binary data */

    DtWalkTableTree (RootTable, DtWriteBinary, NULL, NULL);
    Gbl_TableLength = DtGetFileSize (Gbl_Files[ASL_FILE_AML_OUTPUT].Handle);
}


/*
 * Listing support
 */

/******************************************************************************
 *
 * FUNCTION:    DtDumpBuffer
 *
 * PARAMETERS:  FileID              - Where to write buffer data
 *              Buffer              - Buffer to dump
 *              Offset              - Offset in current table
 *              Length              - Buffer Length
 *
 * RETURN:      None
 *
 * DESCRIPTION: Another copy of DumpBuffer routine (unfortunately).
 *
 * TBD: merge dump buffer routines
 *
 *****************************************************************************/

static void
DtDumpBuffer (
    UINT32                  FileId,
    UINT8                   *Buffer,
    UINT32                  Offset,
    UINT32                  Length)
{
    UINT32                  i;
    UINT32                  j;
    UINT8                   BufChar;


    FlPrintFile (FileId, "Output: [%3.3Xh %4.4d% 3d] ",
        Offset, Offset, Length);

    i = 0;
    while (i < Length)
    {
        if (i >= 16)
        {
            FlPrintFile (FileId, "%23s", "");
        }

        /* Print 16 hex chars */

        for (j = 0; j < 16;)
        {
            if (i + j >= Length)
            {
                /* Dump fill spaces */

                FlPrintFile (FileId, "   ");
                j++;
                continue;
            }

            FlPrintFile (FileId, "%02X ", Buffer[i+j]);
            j++;
        }

        FlPrintFile (FileId, " ");
        for (j = 0; j < 16; j++)
        {
            if (i + j >= Length)
            {
                FlPrintFile (FileId, "\n\n");
                return;
            }

            BufChar = Buffer[(ACPI_SIZE) i + j];
            if (ACPI_IS_PRINT (BufChar))
            {
                FlPrintFile (FileId, "%c", BufChar);
            }
            else
            {
                FlPrintFile (FileId, ".");
            }
        }

        /* Done with that line. */

        FlPrintFile (FileId, "\n");
        i += 16;
    }

    FlPrintFile (FileId, "\n\n");
}


/******************************************************************************
 *
 * FUNCTION:    DtWriteFieldToListing
 *
 * PARAMETERS:  Buffer              - Contains the compiled data
 *              Field               - Field node for the input line
 *              Length              - Length of the output data
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write one field to the listing file (if listing is enabled).
 *
 *****************************************************************************/

void
DtWriteFieldToListing (
    UINT8                   *Buffer,
    DT_FIELD                *Field,
    UINT32                  Length)
{
    UINT8                   FileByte;


    if (!Gbl_ListingFlag || !Field)
    {
        return;
    }

    /* Dump the original source line */

    FlPrintFile (ASL_FILE_LISTING_OUTPUT, "Input:  ");
    FlSeekFile (ASL_FILE_INPUT, Field->ByteOffset);

    while (FlReadFile (ASL_FILE_INPUT, &FileByte, 1) == AE_OK)
    {
        FlWriteFile (ASL_FILE_LISTING_OUTPUT, &FileByte, 1);
        if (FileByte == '\n')
        {
            break;
        }
    }

    /* Dump the line as parsed and represented internally */

    FlPrintFile (ASL_FILE_LISTING_OUTPUT, "Parsed: %*s : %s\n",
        Field->Column-4, Field->Name, Field->Value);

    /* Dump the hex data that will be output for this field */

    DtDumpBuffer (ASL_FILE_LISTING_OUTPUT, Buffer, Field->TableOffset, Length);
}


/******************************************************************************
 *
 * FUNCTION:    DtWriteTableToListing
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write the entire compiled table to the listing file
 *              in hex format
 *
 *****************************************************************************/

void
DtWriteTableToListing (
    void)
{
    UINT8                   *Buffer;


    if (!Gbl_ListingFlag)
    {
        return;
    }

    /* Read the entire table from the output file */

    Buffer = UtLocalCalloc (Gbl_TableLength);
    FlSeekFile (ASL_FILE_AML_OUTPUT, 0);
    FlReadFile (ASL_FILE_AML_OUTPUT, Buffer, Gbl_TableLength);

    /* Dump the raw table data */

    AcpiOsRedirectOutput (Gbl_Files[ASL_FILE_LISTING_OUTPUT].Handle);

    AcpiOsPrintf ("\n%s: Length %d (0x%X)\n\n",
        ACPI_RAW_TABLE_DATA_HEADER, Gbl_TableLength, Gbl_TableLength);
    AcpiUtDumpBuffer2 (Buffer, Gbl_TableLength, DB_BYTE_DISPLAY);

    AcpiOsRedirectOutput (stdout);
}
