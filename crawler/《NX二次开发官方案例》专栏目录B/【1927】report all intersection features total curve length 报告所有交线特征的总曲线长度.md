### 【1927】report all intersection features total curve length 报告所有交线特征的总曲线长度

#### 代码

```cpp
    /*HEAD REPORT_ALL_INTERSECTION_FEATURES_TOTAL_CURVE_LENGTH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志信息。 */  
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
    #define SHOW_DOUBLE(X)    (report_a_double(#X, X))  
    static void report_a_double(char *variable, double value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %f\n", variable, value);  
        uc1601(msg, TRUE);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static int ask_all_features_of_type(tag_t part, char *type, tag_t **feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature_of_type(part, type, feat)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void flag_object(tag_t object, char *msg)  
    {  
        double  
            loc[3];  
        if (UF_OBJ_ask_name_origin(object, loc))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        display_temporary_text(msg, loc);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static double report_curve_array_total_length(int n_curves, tag_t *curves)  
    {  
        int  
            ii;  
        double  
            length,  
            total_length = 0;  
        char  
            msg[133];  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curves[ii], 0.0, 1.0,  
                UF_MODL_UNITS_PART, &length));  
            sprintf(msg, "%f", length);  
            flag_object(curves[ii], msg);  
            total_length = total_length + length;  
        }  
        set_highlight_object_array(n_curves, curves, TRUE);  
        SHOW_DOUBLE(total_length);  
        set_highlight_object_array(n_curves, curves, FALSE);  
        return total_length;  
    }  
    static double report_intersection_features_total_curve_length(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            *curves;  
        double  
            length;  
        if (!UF_CALL(UF_CURVE_ask_feature_curves(feat, &n, &curves))  
            && (n > 0))  
        {  
            length = report_curve_array_total_length(n, curves);  
            UF_free(curves);  
        }  
        return length;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats,  
            part = UF_PART_ask_display_part();  
        double  
            overall_length = 0;  
        if ((n_feats = ask_all_features_of_type(part, "INTERSECTION_CURVES",  
            &feats)) > 0)  
        {  
            for (ii = 0; ii < n_feats; ii++)  
                overall_length = overall_length +  
                    report_intersection_features_total_curve_length(feats[ii]);  
            UF_free(feats);  
            SHOW_DOUBLE(overall_length);  
        }  
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

> 这段NX二次开发代码的功能是计算零件中所有交线特征的总长度，并在NX界面中显示。
>
> 主要思路包括：
>
> 1. 遍历零件中的所有特征，找到类型为"INTERSECTION_CURVES"的特征。
> 2. 对每个交线特征，获取其包含的所有曲线，并计算每条曲线的长度。
> 3. 将每条曲线的长度累加，得到交线特征的总长度，并在曲线旁边显示长度。
> 4. 将所有交线特征的总长度累加，得到整个零件中交线的总长度，并在NX界面中显示。
> 5. 使用高亮显示来标记出被测量的曲线。
> 6. 输出错误信息到系统日志和NX窗口。
> 7. 提供卸载函数。
>
> 总的来说，这段代码实现了对零件中交线特征曲线长度的计算和显示，为用户提供了一种便捷的测量工具。
>
