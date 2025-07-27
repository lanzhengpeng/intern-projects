### 【0196】ask extrusion 问挤出

#### 代码

```cpp
    /*HEAD ASK_EXTRUSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，翻译如下：

注意：UF_print_syslog是在V18版本中新增的。

不要回答多余的内容，只需翻译即可。 */  
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
        logical  
            region_specified,  
            solid_creation;  
        int  
            count,  
            ii,  
            jj,  
            n_curves;  
        tag_t  
            body = NULL_TAG,  
            *curves,  
            feat,  
            part = UF_PART_ask_display_part();  
        double  
            direction[3],  
            region_point[3];  
        uf_list_p_t  
            list;  
        char  
            *limits[2],  
            *offsets[2],  
            *feat_name,  
            *feat_type,  
            *taper_angle;  
        UF_MODL_SWEEP_TRIM_object_p_t  
            trim_ptr;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_ask_body_feats( body, &list));  
            UF_CALL( UF_MODL_ask_list_count( list, &count));  
            printf("Body %d has %d features\n", body, count);  
            for (ii = 0; ii < count; ii++)  
            {  
                UF_CALL( UF_MODL_ask_list_item( list, ii, &feat));  
                UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
                UF_CALL( UF_MODL_ask_feat_name( feat, &feat_name));  
                printf("\n  %d.  %s - %s\n", ii+1, feat_name, feat_type);  
                if (!strcmp( feat_type, "SWP104") &&  
                    !UF_CALL( UF_MODL_ask_extrusion( feat, &n_curves, &curves,  
                        &trim_ptr, &taper_angle, limits, offsets, region_point,  
                        &region_specified, &solid_creation, direction)))  
                {  
                    UF_free(curves);  
                    if (trim_ptr) UF_free(trim_ptr);  
                    UF_free(taper_angle);  
                    for (jj = 0; jj < 2; jj++) UF_free(limits[jj]);  
                    for (jj = 0; jj < 2; jj++) UF_free(offsets[jj]);  
                }  
                UF_free( feat_name);  
                UF_free( feat_type);  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历当前部件中的所有体，并打印出每个体中的特征信息，特别是挤压特征。
>
> 代码的流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 2. 定义了一个函数ask_next_body，用于遍历部件中的所有体，并返回下一个体。
> 3. 定义了主函数do_it，在该函数中：首先获取当前显示的部件。然后使用ask_next_body函数遍历所有体。对于每个体，获取其中的所有特征，并打印特征的名称和类型。如果特征是挤压特征，还会打印出挤压特征的详细信息，包括轮廓曲线、修剪对象、倾斜角度等。
> 4. 首先获取当前显示的部件。
> 5. 然后使用ask_next_body函数遍历所有体。
> 6. 对于每个体，获取其中的所有特征，并打印特征的名称和类型。
> 7. 如果特征是挤压特征，还会打印出挤压特征的详细信息，包括轮廓曲线、修剪对象、倾斜角度等。
> 8. 定义了ufusr函数，该函数在NX中加载该UF时会被调用。在该函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义了ufusr_ask_unload函数，该函数在卸载该UF时会被调用。该函数返回立即卸载。
>
> 综上，这段代码的主要功能是遍历当前部件中的所有体，并打印出每个体中的挤压特征信息，以用于二次开发中的特征信息查询和分析。
>
