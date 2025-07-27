### 【1835】recreate reblend face feature 重创建重新混合面特征

#### 代码

```cpp
    /*HEAD RECREATE_REBLEND_FACE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_modl_dfo.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static void delete_a_feature(tag_t feature)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        UF_CALL(UF_MODL_put_list_item(feat_list, feature));  
        UF_CALL(UF_MODL_delete_feature(feat_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            feat;  
        UF_MODL_reblend_face_data_t  
            reblend_data;  
        while ((feat = select_a_feature("Select Reblend Face")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_reblend_face(feat, &reblend_data))) continue;  
            delete_a_feature(feat);  
            UF_CALL(UF_MODL_create_reblend_face(&reblend_data, &feat));  
            for (ii = 0; ii < reblend_data.num_chains; ii++)  
                if (reblend_data.chain_data[ii].n_faces > 0)  
                    UF_free(reblend_data.chain_data[ii].faces_eid);  
            if (reblend_data.num_chains > 0) UF_free(reblend_data.chain_data);  
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

> 这段代码是用于NX Open API开发的二次开发代码，主要实现了以下功能：
>
> 1. 初始化NX系统。
> 2. 提示用户选择要重新融合的实体面。
> 3. 查询该面是否可以重新融合。
> 4. 如果可以，则删除原来的面，并使用查询得到的重新融合数据创建一个新的面。
> 5. 释放重新融合数据占用的内存。
> 6. 重复以上过程，直到用户不再选择面。
> 7. 终止NX系统。
>
> 代码流程包括以下步骤：
>
> 1. 包含必要的头文件。
> 2. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义选择特征函数select_a_feature，用于让用户选择一个特征。
> 4. 定义删除特征函数delete_a_feature，用于删除一个特征。
> 5. 定义主函数do_it，用于实现重新融合面的逻辑。
> 6. 定义ufusr函数，作为程序的入口，调用主函数。
> 7. 定义ufusr_ask_unload函数，用于卸载程序时立即释放内存。
>
> 该代码实现了NX中重新融合面的功能，通过重复提示用户选择面，删除并重新创建，实现了对面的修改。同时，代码采用了NX Open API的UF模块进行操作，并且包含了错误处理机制。
>
