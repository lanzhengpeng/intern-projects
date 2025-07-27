### 【0631】create circular iset 在UGNX中，创建圆形实例集的功能是通过UF_MODL_create_circular_iset函数实现的，用于创建一个圆形排列的实例集

#### 代码

```cpp
    /*HEAD CREATE_CIRCULAR_ISET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的函数，用于打印系统日志。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void do_it(void)  
    {  
        uf_list_p_t list;  
        double      location[3];  
        double      axes[9];  
        tag_t       feat;  
        tag_t       feature_obj_id;  
        tag_t       body_id;  
        char        num_str[] = "12";  
        char        angle_str[] = "30";  
        ask_wcs_info(location, axes);  
        UF_CALL( UF_MODL_create_list( &list));  
        feat = select_a_feature("Select feature for circular array");  
        UF_CALL( UF_MODL_put_list_item( list, feat));  
        UF_CALL( UF_MODL_ask_feat_body( feat, &body_id));  
        UF_CALL( UF_MODL_active_part( body_id, TRUE ));  
        UF_CALL( UF_MODL_create_circular_iset( 0, location, &axes[6], num_str,  
            angle_str, list, &feature_obj_id));  
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

> 这段NX二次开发代码主要实现了创建圆形阵列特征的功能，并包括以下主要步骤：
>
> 1. 包含必要的NX Open API头文件。
> 2. 定义了一个报告错误的函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个选择特征的函数select_a_feature，用于让用户选择一个特征。
> 4. 定义了一个查询WCS信息的函数ask_wcs_info，用于获取当前工作坐标系的信息。
> 5. 定义了主函数do_it，其中实现了创建圆形阵列特征的主要逻辑：创建一个特征列表list选择一个特征feat并添加到列表中获取特征所在实体body_id激活实体调用UF_MODL_create_circular_iset函数，使用特征列表、位置、角度等信息创建圆形阵列特征
> 6. 创建一个特征列表list
> 7. 选择一个特征feat并添加到列表中
> 8. 获取特征所在实体body_id
> 9. 激活实体
> 10. 调用UF_MODL_create_circular_iset函数，使用特征列表、位置、角度等信息创建圆形阵列特征
> 11. 定义了ufusr函数，作为NX Open程序的入口，调用do_it函数实现功能。
> 12. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序结束后不需要保留内存。
>
> 总体来说，这段代码实现了基于用户选择的一个特征，在指定位置和角度创建圆形阵列特征的功能，展示了NX Open API在特征建模方面的应用。
>
