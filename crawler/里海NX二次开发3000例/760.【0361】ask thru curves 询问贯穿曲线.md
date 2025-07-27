### 【0361】ask thru curves 询问贯穿曲线

#### 代码

```cpp
    /*HEAD ASK_THRU_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的注释，UF_print_syslog是一个在V18版本中新增的函数。根据要求，只回答翻译，不要添加任何其他内容，因此翻译为：

UF_print_syslog是在V18版本中新增的。 */  
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
            c_flag[2],  
            ii,  
            n,  
            patch,  
            vdegree,  
            vstatus;  
        tag_t  
            body = NULL_TAG,  
            c_face_id[2],  
            feat,  
            part = UF_PART_ask_display_part();  
        double  
            tol[3],  
            value[6];  
        uf_list_p_t  
            list;  
        char  
            *feat_type;  
        UF_STRING_t  
            s_section,  
            s_spine;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_body_feats(body, &list));  
            UF_CALL(UF_MODL_ask_list_count(list, &n));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(list, ii, &feat));  
                UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
                if (!strcmp(feat_type, "THRU_CURVE"))  
                {  
                    if(!UF_CALL(UF_MODL_ask_thru_curves(feat, &s_section, &s_spine,  
                        &patch, &alignment, value, &vdegree, &vstatus, &body_type,  
                        tol, c_face_id, c_flag)))  
                    {  
                        ECHO(patch);  
                        ECHO(body_type);  
                        UF_MODL_free_string_list(&s_section);  
                        UF_MODL_free_string_list(&s_spine);  
                    }  
                }  
                UF_free( feat_type);  
            }  
            UF_CALL(UF_MODL_delete_list(&list));  
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

> 这段代码是NX的二次开发代码，主要功能是遍历当前显示的部件，询问每个实体上的通过曲线特征，并输出其参数。以下是代码的主要功能：
>
> 1. 定义了一个报告错误的函数report_error，用于在调用UF函数出错时输出错误信息。
> 2. 定义了一个询问下一个实体的函数ask_next_body，用于遍历部件中的所有实体。
> 3. 定义了一个do_it函数，用于执行主要逻辑：获取当前显示的部件。遍历部件中的所有实体。对每个实体，获取其上的特征列表。遍历特征列表，如果特征类型是"THRU_CURVE"，则调用UF_MODL_ask_thru_curves函数获取该特征的信息，并输出。释放特征类型字符串。删除特征列表。
> 4. 获取当前显示的部件。
> 5. 遍历部件中的所有实体。
> 6. 对每个实体，获取其上的特征列表。
> 7. 遍历特征列表，如果特征类型是"THRU_CURVE"，则调用UF_MODL_ask_thru_curves函数获取该特征的信息，并输出。
> 8. 释放特征类型字符串。
> 9. 删除特征列表。
> 10. ufusr函数是NX调用的入口函数，在这里初始化UF库，调用do_it函数，然后终止UF库。
> 11. ufusr_ask_unload函数用于卸载用户自定义函数，这里返回立即卸载。
>
> 总的来说，这段代码的主要功能是遍历当前显示的部件，询问每个实体上的通过曲线特征，并输出其参数。
>
