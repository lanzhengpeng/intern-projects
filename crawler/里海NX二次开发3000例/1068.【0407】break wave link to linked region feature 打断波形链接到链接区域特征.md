### 【0407】break wave link to linked region feature 打断波形链接到链接区域特征

#### 代码

```cpp
    /*HEAD BREAK_WAVE_LINK_TO_LINKED_REGION_FEATURE CCC UFUN */  
    /*  
        There is a Problem in the interactive NX interface that causes broken   
        LINKED_REGION features to appear as though they are not broken.  
        This program WILL break the link, but in NX2 and the initial release of NX3  
        you will need to use the WAVE Part Link Browser to see that it is really   
        broken.  The edit parameters dialog will continue to show the BreaK Links  
        button as sensitive, as though the link is still available to break.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_wave.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

请注意，UF_print_syslog是V18版本中新增的。 */  
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
    /* 里海 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG;  
        const int   
            n_seed_faces = 0,  
            n_boundary_faces = 0;  
        const tag_t  
            *seed_faces = NULL,  
            * boundary_faces = NULL,  
            xform = NULL_TAG;  
        const logical  
            update_at_timestamp = FALSE,   
            traverse_interior_edges = FALSE,   
            delete_openings = FALSE;  
        if((feat=select_a_feature("Pick a LINKED_REGION Feature:")) != NULL_TAG)  
        {  
            UF_CALL( UF_WAVE_set_link_region_data ( feat, n_seed_faces, seed_faces,  
                      n_boundary_faces, boundary_faces, xform, update_at_timestamp,  
                                       traverse_interior_edges, delete_openings ));  
        }  
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

> 这段代码是一个NX二次开发程序，主要功能是选择一个LINKED_REGION特征，并调用UF_WAVE_set_link_region_data函数来更改该特征的链接区域数据。下面是该程序的主要步骤：
>
> 1. 初始化和错误报告：代码首先定义了错误报告函数report_error，用于在UF函数调用失败时输出错误信息。然后调用UF_initialize初始化NX环境。
> 2. 特征选择：使用自定义函数select_a_feature让用户选择一个LINKED_REGION特征。该函数调用UF_UI_select_feature获取用户选择的特征。
> 3. 更改特征数据：在do_it函数中，如果用户成功选择了特征，则调用UF_WAVE_set_link_region_data来更改该特征的链接区域数据。其中，所有输入参数都设为默认值。
> 4. 结束和卸载：程序最后调用UF_terminate结束NX环境，并在ufusr_ask_unload中返回立即卸载标志。
>
> 总的来说，这段代码实现了选择LINKED_REGION特征并更改其链接区域数据的基本功能。需要注意的是，在NX2和NX3初期版本中，交互式NX界面存在显示问题，可能需要使用WAVE部件链接浏览器来确认链接是否已断开。
>
