### 【3140】report character fonts in current part 列出当前部件中的字符字体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ugfont.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            missing,  
            n_fonts;  
        tag_t  
            fte = ask_font_table_entity();  
        char  
            *font_dir,  
            fnx_name[MAX_FSPEC_SIZE+1],  
            *name;  
        UF_CALL(UF_translate_variable("UGII_CHARACTER_FONT_DIR", &font_dir));  
        UF_CALL(UF_UGFONT_ask_number_of_fonts(fte, &n_fonts));  
        ECHO("Number of fonts found: %d\n", n_fonts);  
        for (ii = 1; ii <= n_fonts; ii++)  
        {  
            UF_CALL(UF_UGFONT_ask_font_name(fte, ii, &name));  
            ECHO("%d.  %s", ii, name);  
            uc4575(font_dir, 105, name, fnx_name);  
            UF_CALL(UF_CFI_ask_file_exist(fnx_name, &missing));  
            if (missing)  
                ECHO(" (%s file not found)\n", fnx_name);  
            else  
                ECHO("\n");  
            UF_free(name);  
        }  
    }  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是检查NX系统中的字体文件是否存在于指定的字体目录下。以下是代码的主要功能介绍：
>
> 1. 错误报告机制：定义了ECHO和report_error函数，用于输出信息到日志窗口，并报告API调用错误。
> 2. 获取字体表实体：定义了ask_font_table_entity函数，用于遍历当前打开的部件，获取字体表实体。
> 3. 字体检查逻辑：定义了do_it函数，用于获取字体表中的字体数量，遍历每个字体，构建字体文件的路径，然后检查该文件是否存在。
> 4. NX系统初始化与终止：ufusr函数用于初始化NX系统，执行字体检查，然后终止NX系统。
> 5. 卸载函数：ufusr_ask_unload函数返回立即卸载标志。
>
> 总体而言，这段代码实现了对NX系统中的字体文件进行有效性检查的功能，以避免出现字体文件丢失的情况。
>
