### 【0803】create offset midsurface 创建偏移曲面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            seed;  
        double  
            cliff_angle = 5,  // best practice = 5, max suggested = 15  
            percentage_distance = 50;  
        if (((seed = select_a_face("Seed face")) != NULL_TAG) &&  
            prompt_for_two_numbers("Offset Midsurface",  
                "Midsurface Position", "Cliff Angle (deg)",  
                &percentage_distance, &cliff_angle))  
        {  
            UF_CALL(UF_SF_create_offset_midsrf(seed, cliff_angle,  
                percentage_distance, &feat));  
        }  
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

> 这段代码是NX二次开发的用户函数，主要实现了以下功能：
>
> 1. 包含了必要的NX二次开发头文件。
> 2. 定义了一个ECHO宏，用于在NX的日志窗口和系统日志中输出信息。
> 3. 定义了一个UF_CALL宏，用于调用NX API函数，并捕获错误代码。
> 4. 定义了一个report_error函数，用于输出API调用错误信息。
> 5. 定义了一个mask_for_faces函数，用于设置选择模式为选择实体上的任意面。
> 6. 定义了一个select_a_face函数，用于通过对话框让用户选择一个面。
> 7. 定义了一个prompt_for_two_numbers函数，用于通过对话框让用户输入两个数字。
> 8. 定义了一个do_it函数，用于让用户选择一个面，并输入两个数字，然后创建偏置曲面。
> 9. 定义了ufusr函数，这是用户函数的入口点，初始化NX环境，调用do_it函数，然后终止NX环境。
> 10. 定义了一个ufusr_ask_unload函数，用于在卸载用户函数时返回立即卸载。
>
> 总体来说，这段代码通过对话框让用户选择一个面，并输入两个数字，然后根据这些信息创建一个偏置曲面，实现了简单的偏置曲面功能。
>
