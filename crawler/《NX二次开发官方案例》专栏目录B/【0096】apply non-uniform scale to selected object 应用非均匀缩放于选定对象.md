### 【0096】apply non-uniform scale to selected object 应用非均匀缩放于选定对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    /* 里海 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,  
        char * item3, double *number1, double *number2, double *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        double  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1609(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            one = 1,  
            resp,  
            subtype,  
            two = 2,  
            type,  
            zero = 0;  
        tag_t  
            csys = NULL_TAG,  
            obj,  
            point,  
            *scale_feat,  
            xyz[3];  
        double  
            mx[12],  
            org[3],  
            scale[3] = { 1, 1, 1 };  
        char  
            factors[3][UF_MAX_EXP_LENGTH];  
        while (((obj = select_an_object("Select object to tranform")) != NULL_TAG) &&  
            (specify_point("Select invariant scale point", org)) &&  
            (prompt_for_three_numbers("Transformations", "XC-Scale", "YC-Scale",  
                "ZC-Scale", &scale[0], &scale[1], &scale[2])))  
        {  
            csys = select_a_csys("Select CSYS (or Cancel for WCS)");  
            set_undo_mark("Non-uniform scale");  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                for (ii = 0; ii < 3; ii++)  
                    UF_CALL(UF_SO_create_scalar_double(obj, UF_SO_update_within_modeling,  
                        org[ii], &xyz[ii]));  
                UF_CALL(UF_SO_create_point_3_scalars(obj, UF_SO_update_within_modeling,  
                    xyz, &point));  
                for (ii = 0; ii < 3; ii++)  
                    sprintf(&factors[ii][0], "%f", scale[ii]);  
                if (!UF_CALL(UF_MODL_create_scale(UF_SCALE_TYPE_GENERAL, &obj, 1, point,  
                    NULL_TAG, csys, factors, &scale_feat))) UF_free(scale_feat);  
            }  
            else  
            {  
                FTN(uf5944)(&two, scale, org, mx, &resp);  
                WRITENZ(resp);  
                FTN(uf5947)(mx, &obj, &one, &one, &zero, &two, NULL, NULL, &resp);  
                WRITENZ(resp);  
            }  
            csys = NULL_TAG;  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能如下：
>
> 1. 包含头文件，引入了常用的NX Open API模块。
> 2. 定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 3. 定义了UF_CALL宏，用于调用NX Open API函数，并报告错误。
> 4. 定义了report_error函数，用于处理API函数调用错误。
> 5. 定义了select_an_object函数，用于选择一个对象。
> 6. 定义了specify_point函数，用于指定一个点。
> 7. 定义了mask_for_csys函数，用于设置选择坐标系的选择器掩码。
> 8. 定义了select_a_csys函数，用于选择一个坐标系。
> 9. 定义了prompt_for_three_numbers函数，用于提示用户输入三个数字。
> 10. 定义了set_undo_mark函数，用于设置撤销标记。
> 11. 定义了do_it函数，实现了主要功能：选择一个对象、指定一个点、输入缩放比例，然后进行非均匀缩放。
> 12. 定义了ufusr函数，作为程序的入口，初始化、调用do_it函数、终止。
> 13. 定义了ufusr_ask_unload函数，用于卸载用户自定义库。
>
> 综上所述，该代码实现了选择对象、指定点、输入比例，然后对所选对象进行非均匀缩放的功能。
>
