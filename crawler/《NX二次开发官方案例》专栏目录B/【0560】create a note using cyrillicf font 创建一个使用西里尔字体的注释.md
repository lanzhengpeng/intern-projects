### 【0560】create a note using cyrillicf font 创建一个使用西里尔字体的注释

#### 代码

```cpp
    /*HEAD CREATE_A_NOTE_USING_CYRILLICF_FONT CCC UFUN */  
    /*  These same methods should also work for Asian fonts such as Chinese 译:同样的方法也适用于亚洲字体，如中文。 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新增功能。 */  
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
        else return FALSE;  
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
      {0x80,    0x00,    0*6,      0x7F,    0},          /* 1 byte sequence 译:翻译1 byte sequence为"1字节序列"。 */  
      {0xE0,    0xC0,    1*6,     0x7FF,    0x80},       /* 2 byte sequence 译:2 byte sequence翻译为“2字节序列”。 */  
      {0xF0,    0xE0,    2*6,    0xFFFF,    0x800},      /* 3 byte sequence 译:你好，请提供3 byte sequence的具体内容，我将为您翻译。 */  
      /****************************************************************************  
       As the unicode values now occupy only 2 bytes, following lines are commented.  
      {0xF8,    0xF0,    3*6,  0x1FFFFF,    0x10000},     4 byte sequence  
      {0xFC,    0xF8,    4*6, 0x3FFFFFF,    0x200000},    5 byte sequence  
      {0xFE,    0xFC,    5*6,0x7FFFFFFF,    0x4000000},   6 byte sequence  
      ****************************************************************************/  
      {0,0,0,0,0}  
    };  
    /* Convert a unicode string into UTF-8 译:将一个Unicode字符串转换为UTF-8。 */  
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
        return len_string ; /* The number of bytes the UTF8 string occupies 译:UTF-8字符串占用的字节数 */  
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
            if (unicode[ii] == 0xd )  /* carriage return 译:回车 */  
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
                /* Skip the line feed 译:跳过行注释，只回答译文，不要废话。 */  
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
    static void do_it(void)  
    {  
        int  
            n_lines,  
            old_font;  
        tag_t  
            note;  
        double  
            pos[3];  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE] = { "" },  
            **note_text;  
        UF_DRF_lettering_preferences_t  
            lettering_preferences;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_UTF8));  
    /*  Set the general text font to cyrillicf 译:将通用文本字体设置为西里尔文。 */  
        UF_CALL(UF_DRF_ask_lettering_preferences(&lettering_preferences));  
        old_font = lettering_preferences.general_text.cfw.font;  
        lettering_preferences.general_text.cfw.font =  
            ask_font_index_number("cyrillicf");  
        UF_CALL(UF_DRF_set_lettering_preferences(&lettering_preferences));  
        while (prompt_for_text_file_name("Unicode text file", fspec))  
        {  
            n_lines = ask_utf8_data_from_unicode_text_file(fspec, &note_text);  
            if (n_lines > 0)  
            {  
                if (specify_position("Note location", pos))  
                    UF_CALL(UF_DRF_create_note(n_lines, note_text, pos, 0, &note));  
                UF_free_string_array(n_lines, note_text);  
            }  
        }  
    /*  Set the general text font back to original font 译:将通用文本字体恢复为原始字体 */  
        lettering_preferences.general_text.cfw.font = old_font;  
        UF_CALL(UF_DRF_set_lettering_preferences(&lettering_preferences));  
    }  
    /* 里海 */  
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

> 这段NX Open C++代码的主要功能是创建包含西里尔文字符的注释。以下是代码的主要功能介绍：
>
> 1. 引入了NX Open C++的相关头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了写入列表窗口的宏WRITE和WRITE_D。
> 4. 定义了获取字体表实体的函数ask_font_table_entity。
> 5. 定义了获取字体索引号的函数ask_font_index_number。
> 6. 定义了提示用户输入文本文件名的函数prompt_for_text_file_name。
> 7. 定义了Unicode转UTF-8的数组table和函数unicode_to_utf8。
> 8. 定义了从Unicode文本文件获取UTF-8数据的函数ask_utf8_data_from_unicode_text_file。
> 9. 定义了指定位置的函数specify_position。
> 10. 定义了主函数do_it，其中包括以下步骤：设置文本模式为UTF-8。获取当前的标注字体偏好设置。将通用文本字体设置为西里尔文。循环提示用户输入Unicode文本文件，并获取文件中的文本行。如果获取到文本行，提示用户指定注释位置，并创建注释。最后，将通用文本字体恢复为原始字体。
> 11. 设置文本模式为UTF-8。
> 12. 获取当前的标注字体偏好设置。
> 13. 将通用文本字体设置为西里尔文。
> 14. 循环提示用户输入Unicode文本文件，并获取文件中的文本行。
> 15. 如果获取到文本行，提示用户指定注释位置，并创建注释。
> 16. 最后，将通用文本字体恢复为原始字体。
> 17. 定义了ufusr函数，初始化后调用do_it函数，然后终止。
> 18. 定义了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过设置西里尔文字体，并从Unicode文本文件中读取文本，在指定位置创建包含西里尔文字符的注释。
>
