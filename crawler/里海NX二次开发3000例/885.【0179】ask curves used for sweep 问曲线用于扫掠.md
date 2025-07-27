### 【0179】ask curves used for sweep 问曲线用于扫掠

#### 代码

```cpp
    /*HEAD ASK_CURVES_USED_FOR_SWEEP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 版本新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            alignment,  
            body_type,  
            count,  
            err,  
            ii,  
            inter,  
            jj,  
            n_curves,  
            num_prof,  
            num_guide;  
        tag_t  
            body = NULL_TAG,  
            *curves,  
            feat,  
            *guide_curv,  
            part = UF_PART_ask_display_part(),  
            *prof_curv;  
        double  
            tol[3];  
        uf_list_p_t  
            list;  
        char  
            *feat_name,  
            *feat_type;  
        UF_STRING_t  
            guide,  
            section,  
            spine;  
        UF_METHOD_t  
            orientation,  
            scaling;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_ask_body_feats( body, &list));  
            UF_CALL( UF_MODL_ask_list_count( list, &count));  
            printf("\nBody %d has %d features\n", body, count);  
            for (ii = 0; ii < count; ii++)  
            {  
                n_curves = 0;  
                UF_CALL( UF_MODL_ask_list_item( list, ii, &feat));  
                UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
                UF_CALL( UF_MODL_ask_feat_name( feat, &feat_name));  
                printf("  %d.  %s - %s\n", ii+1, feat_name, feat_type);  
                if (!strcmp(feat_type, "SWP104"))  
                {  
                    if (!UF_CALL(UF_MODL_ask_sweep_curves(feat, &num_prof,  
                            &prof_curv, &num_guide, &guide_curv)))  
                    {  
                        WRITE_D(num_prof);  
                        if (num_prof > 0)  
                        {  
                            curves = (tag_t *)UF_allocate_memory(  
                                num_prof * sizeof(tag_t), &err);  
                            if (!err)  
                            {  
                                n_curves = num_prof;  
                                for (jj = 0; jj < n_curves; jj++)  
                                    curves[jj] = prof_curv[jj];  
                            }  
                            UF_free(prof_curv);  
                        }  
                        WRITE_D(num_guide);  
                        if (num_guide > 0) UF_free(guide_curv);  
                    }  
                }  
                if (!strcmp(feat_type, "SKIN"))  
                {  
                    if (!UF_CALL(UF_MODL_ask_sweep(feat, &guide, &section, &spine,  
                        &orientation, &scaling, &alignment, &inter, &body_type,  
                        tol)))  
                    {  
                        WRITE_D(guide.num);  
                        UF_MODL_free_string_list(&guide);  
                        WRITE_D(section.num);  
                        if (section.num > 0)  
                        {  
                            for (jj = 0; jj < section.num; jj++)  
                                n_curves += section.string[jj];  
                            curves = (tag_t *)UF_allocate_memory(  
                                n_curves * sizeof(tag_t), &err);  
                            if (!err)  
                                for (jj = 0; jj < n_curves; jj++)  
                                    curves[jj] = section.id[jj];  
                            else  
                                n_curves = 0;  
                            UF_MODL_free_string_list(&section);  
                        }  
                        WRITE_D(spine.num);  
                        UF_MODL_free_string_list(&spine);  
                    }  
                }  
                UF_free(feat_name);  
                UF_free(feat_type);  
                if (n_curves > 0)  
                {  
                    for (jj = 0; jj < n_curves; jj++)  
                        UF_CALL(UF_DISP_set_highlight(curves[jj], TRUE));  
                    uc1601("Section curves are highlighted", TRUE);  
                    for (jj = 0; jj < n_curves; jj++)  
                        UF_CALL(UF_DISP_set_highlight(curves[jj], FALSE));  
                    UF_free(curves);  
                }  
            }  
            UF_CALL(UF_MODL_delete_list(&list));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前显示部件中的所有特征，并识别出扫掠特征。对于每个扫掠特征，它会打印出特征名称、类型以及使用的轮廓曲线和引导曲线的数量。同时，该代码会高亮显示扫掠特征所使用的轮廓曲线，然后取消高亮。具体来说，代码实现了以下功能：
>
> 1. 定义了错误处理函数 report_error()，用于打印错误信息。
> 2. 定义了整数写入列表窗口的函数 write_integer_to_listing_window()。
> 3. 定义了获取下一个实体的函数 ask_next_body()。
> 4. 主函数 do_it() 遍历当前显示部件的所有实体，并获取每个实体的所有特征。
> 5. 对于每个特征，判断其类型。如果为扫掠特征(SWP104)，则获取其轮廓曲线和引导曲线数量。
> 6. 如果特征为蒙皮特征(SKIN)，则获取其引导曲线、截面曲线和脊线曲线。
> 7. 高亮显示扫掠特征所使用的轮廓曲线，然后取消高亮。
> 8. 在 ufusr() 函数中初始化NX环境，执行主函数，然后终止NX环境。
> 9. 提供了卸载函数 ufusr_ask_unload()。
>
> 总的来说，该代码通过遍历特征并判断类型，实现了对扫掠特征使用的轮廓曲线的查询和高亮显示。
>
