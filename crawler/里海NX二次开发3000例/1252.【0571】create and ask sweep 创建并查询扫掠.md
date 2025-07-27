### 【0571】create and ask sweep 创建并查询扫掠

#### 代码

```cpp
    /*HEAD CREATE_AND_ASK_SWEEP CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据原文，UF_print_syslog 是 V18 中的新功能。 */  
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
    static void vec3_init(double vec[3])  
    {  
        vec[0] = 0;  
        vec[1] = 0;  
        vec[2] = 0;  
    }  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        printf("%s = %d, type = %d, subtype = %d\n", name, object, type, subtype);  
    }  
    static int ask_body_feats(tag_t body, tag_p_t *body_feats)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *feats;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        UF_CALL(UF_MODL_ask_list_count(feat_list, &n));  
        *body_feats = (feats = (tag_t *)malloc(n * sizeof(tag_t)));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_ask_list_item(feat_list, ii, &feats[ii]));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        UF_CURVE_line_t  
            line_coords;  
        UF_STRING_t  
            s_guide,  
            s_section,  
            s_spine;  
        UF_METHOD_t  
            orientation,  
            scaling;  
        int  
            alignment = 1,  
            body_type = 1,  
            inter = UF_MODL_LINEAR,  
            n_feats;  
        double  
            tol[3] = { 0, 0, 0 };  
        tag_t  
            body_id,  
            *feats;  
        /*  Create the guide curves 译:创建引导曲线 */  
        UF_MODL_init_string_list(&s_guide);  
        UF_MODL_create_string_list(3, 3, &s_guide);  
        s_guide.num = 3;  
        s_guide.string[0] = 1;  
        s_guide.dir[0] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.end_point[0] = 10;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_guide.id[0] ));  
        s_guide.string[1] = 1;  
        s_guide.dir[1] = 1;  
        line_coords.start_point[1] = 1;  
        line_coords.end_point[1] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_guide.id[1] ));  
        s_guide.string[2] = 1;  
        s_guide.dir[2] = 1;  
        line_coords.start_point[1] = 0;  
        line_coords.start_point[2] = 1;  
        line_coords.end_point[1] = 0;  
        line_coords.end_point[2] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_guide.id[2] ));  
        /*  Create the section curves 译:Create the section curves */  
        UF_MODL_init_string_list(&s_section);  
        UF_MODL_create_string_list(2, 8, &s_section);  
        s_section.num = 2;  
        s_section.string[0] = 4;  
        s_section.dir[0] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.end_point[1] = 1;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[0] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 1;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[1] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[1] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[2] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[3] ));  
        s_section.string[1] = 4;  
        s_section.dir[1] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.start_point[0] = 10;  
        line_coords.end_point[0] = 10;  
        line_coords.end_point[1] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[4] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[5] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[1] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[6] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[7] ));  
        /*  No spine 译:没有脊椎 */  
        UF_MODL_init_string_list(&s_spine);  
        UF_CALL( UF_MODL_create_sweep ( &s_guide, &s_section, &s_spine,  
            &orientation, &scaling, &alignment, &inter, &body_type, UF_NULLSIGN,  
            tol, &body_id ));  
        UF_MODL_free_string_list(&s_guide);  
        UF_MODL_free_string_list(&s_section);  
        UF_MODL_free_string_list(&s_spine);  
        report_tag_type_and_subtype("body_id", body_id);  
        if ((n_feats = ask_body_feats(body_id, &feats)) > 0)  
        {  
            UF_CALL(UF_MODL_ask_sweep ( feats[0], &s_guide, &s_section, &s_spine,  
                &orientation, &scaling, &alignment, &inter, &body_type, tol));  
            UF_MODL_free_string_list(&s_guide);  
            UF_MODL_free_string_list(&s_section);  
            UF_MODL_free_string_list(&s_spine);  
            free(feats);  
        }  
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

> 这段代码是一个用于NX CAD软件的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
> 2. 定义了一个初始化向量函数vec3_init，用于将三维向量初始化为0。
> 3. 定义了一个报告对象类型和子类型的函数report_tag_type_and_subtype，用于打印对象标签、类型和子类型信息。
> 4. 定义了一个询问体特征数量的函数ask_body_feats，用于获取体的特征数量和特征标签。
> 5. do_it函数实现了创建引导曲线、截面曲线、执行扫掠操作，并打印出体的类型和子类型信息。
> 6. ufusr函数是NX的回调函数，用于初始化和执行主体代码。
> 7. ufusr_ask_unload函数用于卸载用户函数。
>
> 整体来说，这段代码实现了创建引导曲线、截面曲线、执行扫掠操作，并打印出体的类型和子类型信息的功能。代码结构清晰，包含错误处理和内存管理，是一个典型的NX二次开发示例。
>
