### 【1835】recreate reblend face feature 重创建重新混合面特征



#### 代码

``````cpp
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
``````



#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是遍历当前显示零件的所有属性并打印出来。以下是代码的主要逻辑。
>
> 1.定义了ECHO和UF_CALL宏，用于打印信息和调用NXAPI，同时报告错误。
>
> 2.定义了report_error函数，用于在调用NXAPI出错时打印错误信息。
>
> 3.定义了report_object_attributes函数，用于遍历对象的所有属性并打印。属性类型包括整型、浮点型、日期时间型、字符串型、引用
> 等。
>
> 4.定义了do_it函数，用于获取当前显示的零件对象，并调用report_object_attributes打印其所有属性。
>
> 5.ufusr函数是二次开发的入口函数，初始化NX后调用do_it函数，然后终止NX。
>
> 6.ufusr_ask_unload函数用于立即卸载二次开发程序。
> 总体来说，这段代码实现了遍历当前显示零件的所有属性并打印的功能，使用了NX的UF_PART和UF_ATTR相关API。

