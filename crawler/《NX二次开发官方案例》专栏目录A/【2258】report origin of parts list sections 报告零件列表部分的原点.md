### 【2258】report origin of parts list sections 报告零件列表部分的原点

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    static void do_it(void)  
    {  
        // Is there a parts list?  
        int  
            n_plists;  
        tag_t  
            plist,  
            *plists;  
        UF_CALL(UF_PLIST_ask_tags(&plists, &n_plists));  
        if (n_plists == 0)  
        {  
            ECHO("No parts lists found.\n\n");  
            return;  
        }  
        plist = plists[0];  
        UF_free(plists);  
        ECHO("Parts List: %d\n\n", plist);  
        // Get the visible bits - "sections"  
        int  
            numSections = 0;  
        UF_CALL(UF_TABNOT_ask_nm_sections(plist, &numSections));  
        ECHO("\n\nNumber of sections found: %d\n\n", numSections);  
        // Report where they are  
        for (int inx = 0; inx < numSections; inx++)  
        {  
            tag_t  
                thisSection = NULL_TAG;  
            double  
                origin[3];  
            char  
                msg[257] = { "" };  
            UF_CALL(UF_TABNOT_ask_nth_section(plist, inx, &thisSection));  
            UF_CALL(UF_DISP_set_highlight(thisSection, 1));  
            UF_CALL(UF_DRF_ask_origin(thisSection, origin));  
            sprintf_s(msg, sizeof(msg), "Origin: %f   %f   %f\n\n", origin[0], origin[1], origin[2]);  
            uc1601(msg, 1);  
            UF_CALL(UF_DISP_set_highlight(thisSection, 0));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_ALL_UTF8));  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX Open API的一个示例，用于访问和操作NX中的部件清单（parts list）。
>
> 代码的主要功能如下：
>
> 1. 使用UF_PLIST_ask_tags函数查询当前打开的NX部件中的所有部件清单，并打印第一个清单的标签。
> 2. 使用UF_TABNOT_ask_nm_sections函数查询第一个清单中的所有可见部分（sections），并打印可见部分的数量。
> 3. 遍历每个可见部分，使用UF_TABNOT_ask_nth_section函数获取每个部分的标签。
> 4. 使用UF_DISP_set_highlight函数将每个部分高亮显示，以突出其在图形区域中的位置。
> 5. 使用UF_DRF_ask_origin函数获取每个部分的原始坐标，并打印。
> 6. 最后，使用UF_DISP_set_highlight函数取消高亮显示。
>
> 此外，代码中还包括了一些辅助函数，例如ECHO用于打印信息，report_error用于报告错误，do_it用于执行主要逻辑。
>
> 总体来说，这段代码展示了如何使用NX Open API访问部件清单信息，并对清单中的部分进行可视化。这对于二次开发中需要操作NX部件清单的场景非常有用。
>
