### 【0572】create and clock multiple feature circular iset 创建一个圆形特征实例集，并对其进行计时

#### 代码

```cpp
    /*HEAD CREATE_AND_CLOCK_MULTIPLE_FEATURE_CIRCULAR_ISET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，以下是 UF_print_syslog 的翻译：

UF_print_syslog 是在 V18 中新增的。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
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
        uf_list_p_t  
            list;  
        int  
            ii;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void create_circular_iset(int n_feats, tag_t *feats, double point[3],  
        double dir[3], int number, double angle, tag_t **isets)  
    {  
        int  
            err,  
            ii;  
        char  
            number_exp[10],  
            angle_exp[10];  
        tag_t  
            feat,  
            *items;  
        uf_list_p_t  
            feat_list;  
        sprintf(number_exp, "%d", number);  
        sprintf(angle_exp, "%f", angle);  
        feat_list = make_a_list(n_feats, feats);  
        UF_CALL(UF_MODL_create_circular_iset(0, point, dir, number_exp, angle_exp,  
            feat_list, &feat));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        *isets = (items=(tag_t *)UF_allocate_memory(n_feats*sizeof(tag_t), &err));  
        for (ii = 0; ii < n_feats; ii++)  
            UF_CALL(UF_MODL_ask_instance_iset(feats[ii], &items[ii]));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            feat,  
            *feats,  
            *isets;  
        uf_list_p_t  
            feat_list;  
        double  
            dir[3],  
            point[3];  
        if (((n = select_features("Select features for circular iset", &feats)) > 0)  
            && (specify_vector("Define revolution axis", dir, point)))  
        {  
            create_circular_iset(n, feats, point, dir, 6, 60.0, &isets);  
            UF_free(feats);  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_MODL_ask_instance(isets[ii], &feat_list));  
                UF_CALL(UF_MODL_ask_list_item(feat_list, 2, &feat));  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
                UF_CALL(UF_MODL_clock_instance(feat, "1", "0"));  
            }  
            UF_free(isets);  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是基于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了选择特征函数select_features，用于让用户选择特征并返回特征数组。
> 3. 定义了指定向量函数specify_vector，用于让用户指定一个向量。
> 4. 定义了创建列表函数make_a_list，用于根据特征数组创建特征列表。
> 5. 定义了创建圆形实例集函数create_circular_iset，根据输入的特征、点、方向、数量和角度，创建圆形实例集。
> 6. 在do_it函数中，首先让用户选择特征，然后指定一个向量作为旋转轴，接着创建圆形实例集，最后对每个实例集进行实例化并更新显示。
> 7. ufusr函数是二次开发的入口函数，初始化后调用do_it函数，完成后再终止。
> 8. ufusr_ask_unload函数返回立即卸载，表示二次开发模块不需要持久化。
>
> 综上所述，这段代码的主要功能是根据用户选择的特征和旋转轴，创建圆形实例集并进行实例化，实现了特征阵列化的功能。
>
