### 【1145】edit edge blend radius 编辑边混合半径

#### 代码

```cpp
    /*HEAD EDIT_EDGE_BLEND_RADIUS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我了解到UF_print_syslog是V18版本中新增的函数。该函数用于打印系统日志。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            blend;  
        UF_MODL_edge_blend_data_t  
            blend_data;  
        char  
            brad[256];  
        while (((blend = select_a_feature("Select an edge blend")) != NULL_TAG) &&  
                !UF_CALL(UF_MODL_ask_edge_blend(blend, &blend_data)))  
        {  
            /*WRITE_D(blend_data.number_edges);*/  
            for (ii = 0; ii < blend_data.number_edges; ii++)  
                if (blend_data.edge_data[ii].number_points > 0)  
                    UF_free(blend_data.edge_data[ii].point_data);  
            strcpy(brad,  blend_data.blend_radius);  
            if (prompt_for_text("Enter New Radius", brad))  
            {  
                strcpy(blend_data.blend_radius, brad);  
                UF_CALL(UF_MODL_edit_edge_blend(blend, &blend_data ));  
            }  
            UF_free(blend_data.edge_data);  
                UF_CALL(UF_MODL_update());  
        }  
        uc1601("Done", TRUE);  
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

> 这段代码是用于NX软件的二次开发，主要功能是修改边界的融合半径。具体来说：
>
> 1. 代码首先定义了错误报告函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. select_a_feature函数用于选择一个特征，这里指边界融合特征。
> 3. prompt_for_text函数用于提示用户输入文本，用于获取新的融合半径值。
> 4. do_it函数是主逻辑函数，遍历选择的所有边界融合特征，获取其数据，释放点数据内存，提示用户输入新半径值，并修改融合半径。
> 5. ufusr函数是NX二次开发的入口函数，初始化后调用do_it函数，完成后再终止。
> 6. ufusr_ask_unload函数用于立即卸载二次开发程序。
>
> 总体而言，这段代码实现了选择边界融合特征并修改其融合半径的功能，通过NX提供的二次开发接口实现。
>
