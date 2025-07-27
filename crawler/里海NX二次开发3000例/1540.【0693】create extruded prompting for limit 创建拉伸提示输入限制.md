### 【0693】create extruded prompting for limit 创建拉伸提示输入限制

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[4];  
        opts.num_mask_triples = 4;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static uf_list_p_t make_a_list( int count, tag_t *item_array)  
    {  
        uf_list_p_t list;  
        int         ii;  
        UF_CALL( UF_MODL_create_list( &list));  
        for( ii = 0; ii < count; ii++)  
            UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));  
        return( list);  
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
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
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
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            n_feats;  
        char  
            *limits[2];  
        tag_t  
            body,  
            feat,  
            *objects;  
        double  
            start = 0.0,  
            end = 0.0,  
            dir[3],  
            point[3];  
        uf_list_p_t  
            curves,  
            feats;  
        while (((n = select_curves("Select curves to extrude", &objects)) > 0) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            curves = make_a_list(n, objects);  
            UF_free(objects);  
            prompt_for_two_numbers("Enter values", "Start", "End", &start, &end);  
            limits[0] = calloc(20, sizeof(char));  
            limits[1] = calloc(20, sizeof(char));  
            sprintf(limits[0], "%f", start);  
            sprintf(limits[1], "%f", end);  
            UF_CALL(UF_MODL_create_extruded(curves, "0.0", limits, point, dir,  
                UF_NULLSIGN, &feats));  
            UF_CALL(UF_MODL_ask_list_count(feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(feats, ii, &feat));  
                UF_CALL(UF_MODL_ask_feat_body(feat, &body));;  
                UF_CALL(UF_OBJ_set_name(body, "Extruded"));  
            }  
            UF_CALL(UF_MODL_delete_list(&curves));  
            UF_CALL(UF_MODL_delete_list(&feats));  
            free(limits[0]);  
            free(limits[1]);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个用于NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出NX函数调用失败的错误信息。
> 2. 定义了一个选择曲线的函数select_curves，可以提示用户选择曲线对象，并返回选择的对象数组。
> 3. 定义了一个指定向量的函数specify_vector，可以提示用户指定一个点和一个方向，以确定一个向量。
> 4. 定义了一个创建列表的函数make_a_list，可以将一组对象放入一个列表中。
> 5. 定义了一个提示输入两个数字的函数prompt_for_two_numbers。
> 6. 定义了一个主函数do_it，其中包含以下逻辑：循环提示用户选择曲线，直到用户取消。对每批曲线，提示用户指定拉伸方向和基点，以及拉伸的起始和结束位置。调用NX的拉伸功能，将曲线拉伸成实体，并设置实体的名称为"Extruded"。
> 7. 循环提示用户选择曲线，直到用户取消。
> 8. 对每批曲线，提示用户指定拉伸方向和基点，以及拉伸的起始和结束位置。
> 9. 调用NX的拉伸功能，将曲线拉伸成实体，并设置实体的名称为"Extruded"。
> 10. 定义了ufusr函数作为NX的二次开发入口，在初始化NX后调用do_it函数，最后在结束前终止NX。
> 11. 定义了卸载函数ufusr_ask_unload，用于在卸载时立即终止NX。
>
> 总体来说，这段代码通过NX的二次开发接口，实现了交互式的曲线拉伸功能，用户可以方便地选择曲线、指定拉伸方向和拉伸长度，生成拉伸实体。代码结构清晰，易于理解和使用。
>
