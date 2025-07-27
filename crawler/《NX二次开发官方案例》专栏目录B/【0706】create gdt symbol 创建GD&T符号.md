### 【0706】create gdt symbol 创建GD&T符号

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
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
    static int mask_for_drf_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_line_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_line_pos(char *prompt, UF_DRF_object_p_t line,  
        double cp[3])  
    {  
        int  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_lines, NULL, &resp,  
            &line->object_tag, cp, &line->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(line->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            gdt;  
        double  
            loc[3],  
            mpr[70],  
            origin[3];  
        char  
            dia[27],  
            rad[27],  
            txt[1][132+1]={"<&2><+><E><+>"};  
        UF_DRF_object_t  
            object;  
        UF_DRF_init_object_structure(&object);  
        while (select_drf_line_pos("Select a line for GD&T", &object, loc)  
            && specify_position("Indicate origin", origin))  
        {  
            UF_CALL(UF_DRF_create_gdt_symbol(1, txt, origin,  
                   UF_DRF_leader_type_ext_line, UF_DRF_leader_attach_object,  
                   &object, loc, UF_DRF_frame_upper_left, &gdt));  
        //  This may change the Leader to horizontal or vertical - see PR 2175132  
            if (ask_yes_or_no("See PR 2175132", "Ask and Set Preferences?"))  
            {  
                UF_CALL(UF_DRF_ask_object_preferences(gdt, mpi, mpr, rad, dia));  
                UF_CALL(UF_DRF_set_object_preferences(gdt, mpi, mpr, rad, dia));  
            }  
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

> 这段代码是NX的二次开发示例，主要实现以下功能：
>
> 1. 错误报告：定义了一个错误报告函数report_error，用于在调用NX API出错时输出错误信息。
> 2. 选择线条：定义了一个选择线条的函数select_drf_line_pos，用于让用户选择一个线条，并获取线条的位置信息。
> 3. 指定位置：定义了一个指定位置的函数specify_position，用于让用户指定一个点的位置。
> 4. 询问用户：定义了一个询问用户是/否的函数ask_yes_or_no。
> 5. 创建GD&T符号：在函数do_it中，循环让用户选择线条和指定位置，然后调用UF_DRF_create_gdt_symbol API创建GD&T符号。
> 6. 询问设置参数：在创建GD&T符号后，询问用户是否要询问和设置参数，如果是，则调用UF_DRF_ask_object_preferences和UF_DRF_set_object_preferences API来询问和设置参数。
> 7. 初始化和结束：在ufusr函数中，初始化NX，调用do_it函数，然后结束NX。
> 8. 卸载函数：定义了一个卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了GD&T符号的创建和参数设置，通过交互方式获取用户的选择和输入。
>
