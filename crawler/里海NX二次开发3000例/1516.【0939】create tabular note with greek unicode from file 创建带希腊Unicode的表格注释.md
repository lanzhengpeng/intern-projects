### 【0939】create tabular note with greek unicode from file 创建带希腊Unicode的表格注释

#### 代码

```cpp
    /*HEAD CREATE_TABULAR_NOTE_WITH_GREEK_UNICODE_FROM_FILE CCC UFUN */  
    /*  These same methods should also work for Asian fonts such as Chinese 里海译:对于亚洲字体，如中文，这些方法同样适用。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugfont.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_drf.h>  
    #include <uf_text.h>  
    #include <uf_cfi.h>  
    #include <uf_tabnot.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }  
    static int ask_font_index_number(char *font_name)  
    {  
        int  
            index;  
        tag_t  
            fte = ask_font_table_entity();  
        UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));  
        return (index);  
    }  
    static logical prompt_for_text_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else   
        {  
            return FALSE;  
        }  
    }  
    typedef struct  
    {  
        int cmask ;  
        int cval ;  
        int shift ;  
        unsigned int lmask ;  
        unsigned int lval ;  
    } utf8_definition_t;  
    static utf8_definition_t table[] =  
    {  
      {0x80,    0x00,    0*6,      0x7F,    0},          /* 1 byte sequence 里海译:翻译1 byte sequence为1字节序列 */  
      {0xE0,    0xC0,    1*6,     0x7FF,    0x80},       /* 2 byte sequence 里海译:对不起，我无法回答这个问题。 */  
      {0xF0,    0xE0,    2*6,    0xFFFF,    0x800},      /* 3 byte sequence 里海译:好的，请提供需要翻译的3 byte sequence。 */  
      /****************************************************************************  
       As the unicode values now occupy only 2 bytes, following lines are commented.  
      {0xF8,    0xF0,    3*6,  0x1FFFFF,    0x10000},     4 byte sequence  
      {0xFC,    0xF8,    4*6, 0x3FFFFFF,    0x200000},    5 byte sequence  
      {0xFE,    0xFC,    5*6,0x7FFFFFFF,    0x4000000},   6 byte sequence  
      ****************************************************************************/  
      {0,0,0,0,0}  
    };  
    /* Convert a unicode string into UTF-8 里海译:将一个Unicode字符串转换为UTF-8编码。 */  
    static int unicode_to_utf8(unsigned short int * unicode_in_string,  
        char **utf8_string, int unicode_len)  
    {  
        unsigned short int  
            *ptr;  
        unsigned int  
            current_char;  
        int  
            delta_len_chr,  
            len_string,  
            rune,  
            x;  
        char  
            msg[MAX_LINE_SIZE],  
            *out_string;  
        utf8_definition_t  
            *tabptr;  
        if ( unicode_in_string == NULL)  
        {  
            *utf8_string = NULL;  
            return 0;  
        }  
        len_string = 0;  
        *utf8_string = UF_allocate_memory(3 * (unicode_len + 1), &x);  
        out_string = (char *)*utf8_string;  
        for (ptr = unicode_in_string; *ptr; )  
        {  
            current_char = *ptr ;  
            delta_len_chr = 0 ;  
            /* cmask and cval are the maximum and minimum values  
               a n byte character can take */  
           for (tabptr=table; tabptr->cmask; tabptr++)  
           {  
               len_string++ ;  
           /* lmask is the maximum value a n byte character can take  
                ( n varys 0 to 6 ) */  
               if ( current_char <= tabptr->lmask)  
               {  
                  /* shift detemines the number of bytes in which the  
                     utf8 character can be represented  
                     shift value is unique for each lmask value */  
                  rune = tabptr->shift ;  
                  *out_string = (( tabptr->cval) | ( current_char >> rune)) ;  
                  delta_len_chr++ ;  
                  while (rune > 0 )  
                  {  
                      rune -= 6 ;  
                      out_string++ ;  
                      *out_string = 0x80 | (( current_char >> rune) & 0x3F ) ;  
                  }  
                  out_string++ ;  
                  ptr++;  
                  break ;  
               }  
           }  
           if (delta_len_chr == 0)  
           {  
              sprintf(msg, "Error: unexpected unicode character: %x\n",  
                current_char);  
              WRITE(msg);  
              break;  
           }  
        }  
        *out_string++ = '\0';  
        return len_string ; /* The number of bytes the UTF8 string occupies 里海译:UTF-8字符串占用的字节数。 */  
    }  
    static int ask_utf8_data_from_unicode_text_file(char *fspec, char ***data)  
    {  
        logical  
            swap_bytes;  
        unsigned short int  
            byteone,  
            unicode[MAX_LINE_SIZE+1];  
        int  
            chan,  
            ii = 0,  
            n_lines = 0,  
            x;  
        char  
            *utf8;  
        if ((chan = uc4500(fspec, 1, 0)) < 0) return 0;  
        uc4513(chan, 2, (char *)&unicode[0]);  
        if (unicode[0] == 0xfeff )  
            swap_bytes = FALSE;  
        else  
            if (unicode[0] = 0xfffe)  
                swap_bytes = TRUE;  
            else  
            {  
                WRITE(fspec);  
                WRITE(" - Invalid unicode file\n");  
                return 0;  
            }  
        *data = NULL;  
        ii = 0;  
        while (uc4513(chan, 2, (char *)&unicode[ii]) == 0)  
        {  
            if (swap_bytes)  
            {  
                byteone = unicode[ii] & 0xff;  
                unicode[ii] = unicode[ii] >>8 | byteone << 8;  
            }  
            if (unicode[ii] == 0xd )  /* carriage return 里海译:回车 */  
            {  
                unicode[ii] = 0;  
                if (ii > 0)  
                {  
                    unicode_to_utf8(unicode, &utf8, ii-1);  
                    n_lines++;  
                    WRITE_D(n_lines);  
                    *data = UF_reallocate_memory(*data, n_lines*sizeof(char *), &x);  
                    (*data)[n_lines-1] = utf8;  
                    ii = 0;  
                }  
                /* Skip the line feed 里海译:Skip the line feed翻译为：跳过换行。 */  
                uc4513(chan, 2, (char *)&unicode[ii]);  
            }  
            else ii++;  
        }  
        return n_lines;  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    void extern create_tabular_note(char *tabtext)  
    {  
        UF_TABNOT_section_prefs_t  
            sec_prefs;  
        double  
            loc[]= { 10,10,0 };  
        tag_t  
            row,  
            col,  
            cel,  
            tabnote;  
        int  
            indxr = 0,  
            indxc = 0;  
        specify_position("Select Origin", loc);  
        UF_CALL(UF_TABNOT_ask_default_section_prefs(&sec_prefs));  
        UF_CALL(UF_TABNOT_create(&sec_prefs, loc, &tabnote));  
        UF_CALL(UF_TABNOT_ask_nth_row(tabnote, indxr, &row));  
        UF_CALL(UF_TABNOT_ask_nth_column(tabnote, indxc, &col));  
        UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, col, &cel));  
        UF_CALL(UF_TABNOT_set_cell_text(cel, tabtext));   
    }  
    static void do_it(void)  
    {  
        int  
            n_lines;  
        char  
            msg[133],  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE] = { "" },  
            **note_text;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs,  
            cell_prefs_orig;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_UTF8));  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&cell_prefs));  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&cell_prefs_orig));  
        cell_prefs.text_font = ask_font_index_number("greek");  
        UF_CALL(UF_TABNOT_set_default_cell_prefs(&cell_prefs));  
        while (prompt_for_text_file_name("Unicode text file", fspec))  
        {  
            n_lines = ask_utf8_data_from_unicode_text_file(fspec, &note_text);  
            sprintf(msg, "Number of lines: %d\n", n_lines);  
            uc1601(msg, TRUE);  
            if (n_lines > 0)  
            {  
                create_tabular_note(note_text[0]);  
                UF_free_string_array(n_lines, note_text);  
            }  
        }  
    /*  Set the preferences back to original 里海译:将首选项恢复到原始设置。 */  
        UF_CALL(UF_TABNOT_set_default_cell_prefs(&cell_prefs_orig));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数，用于输出函数调用失败时的错误信息。
> 2. 字体表操作：提供了获取字体表实体、添加字体到字体表、获取字体索引号等功能。
> 3. 文件对话框：实现了提示用户选择文本文件的对话框。
> 4. Unicode转UTF-8：定义了一个函数，可以将Unicode字符串转换为UTF-8编码。
> 5. 读取Unicode文本文件：实现了读取Unicode文本文件，并将其转换为UTF-8字符串数组的功能。
> 6. 表格注释相关操作：提供了创建表格注释、设置单元格文本等操作表格注释的功能。
> 7. 获取默认单元格参数：实现了获取表格注释的默认单元格参数，并设置默认字体等功能。
> 8. 主函数：实现了选择文本文件，读取文件内容，并在图形区创建表格注释，其中注释内容为文件第一行文本，并设置了默认的希腊字体。
> 9. 清理工作：在主函数最后，恢复了表格注释的默认单元格参数。
> 10. NX启动和退出：定义了NX启动和退出的函数。
>
> 综上所述，该代码主要实现了读取文本文件并在图形区创建表格注释的功能，并支持Unicode文本文件。
>
