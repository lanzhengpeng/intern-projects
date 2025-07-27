### 【3106】edit multi line note from grip pre v16 用于在GRIP程序中编辑已有注释

#### 代码

```cpp
    /*HEAD EDIT_MULTI_LINE_NOTE_FROM_GRIP CCC UFUN */  
    /*  
        NOTE:  Methods demonstrated in this program are obsoleted by the new GRIP  
               command EDTEXT intoduced in V16.0.  
        This is a generic UG/Open API (Ufunc) program which can be called whenever  
        it is desired to edit the text of an existing note in a GRIP program.  You  
        pass in the note entity, a string array containing up to 10 lines of new  
        text, and the number of lines of text.  The string array must be  
        dimensioned (10,132).  
        Here is sample GRIP code demonstating how to use the program:  
            entity/ note_ent  
            string/ note_text(10,132)  
            number/ n_lines  
            data/ n_lines, 1  
            data/ note_text, 'LINE 1', 'LINE 2', 'LINE 3', 'LINE 4', 'LINE 5', $  
                             'LINE 6', 'LINE 7', 'LINE 8', 'LINE 9', 'LINE 10'  
            grargs/ note_ent, note_text, n_lines  
            mask/ 25  
    a10:    ident/ 'Select note to edit', note_ent, resp  
            ifthen/ resp > 2  
                xspawn/ ufun, '/path/to/ufunc/grip_edit_note'  
                n_lines = n_lines + 1  
                if/ n_lines == 11, n_lines = 1  
                jump/ a10:  
            endif  
            halt  
    */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是在V18版本中新增的，只需回答翻译，不要添加其他无关内容。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            a_note;  
        char  
            text[10][133];  
        double  
            n;  
        UF_args_t  
            args[3];  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &a_note;  
        args[1].type = UF_TYPE_CHAR_ARRAY;  
        args[1].length = 10;  
        args[1].address = text;  
        args[2].type = UF_TYPE_DOUBLE;  
        args[2].length = 0;  
        args[2].address = &n;  
        UF_CALL(UF_ask_grip_args(3, args));  
        uc5566(a_note, 1, (int) n, text);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要用于编辑现有注释中的文本。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个do_it函数，用于获取传入的参数，包括注释实体、文本数组以及行数，并调用uc5566函数进行注释文本的编辑。
> 3. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 4. 定义了ufusr_ask_unload函数，用于设置卸载模式为立即卸载。
> 5. 使用了宏定义UF_CALL来简化错误处理。
> 6. 使用了UF_args_t结构体来获取传入参数。
> 7. 使用了tag_t类型来表示注释实体。
> 8. 使用了数组来表示多行文本。
> 9. 使用了double类型来表示行数。
> 10. 调用了uc5566函数进行注释文本的编辑，这个函数需要根据注释实体、行数和文本数组来修改注释内容。
>
> 总的来说，这段代码实现了注释文本的编辑功能，包括参数获取、错误处理、注释文本的修改等。
>
