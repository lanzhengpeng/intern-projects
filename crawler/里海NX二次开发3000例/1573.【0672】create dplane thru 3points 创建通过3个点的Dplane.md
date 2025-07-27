### 【0672】create dplane thru 3points 创建通过3个点的Dplane

#### 代码

```cpp
    /*HEAD CREATE_DPLANE_THRU_3POINTS CCC UFUN */  
    #include <stdlib.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_defs.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t select_an_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        double  
            ref_point[3] = {0,0,0};  
        int  
            num_dplanes,  
            point_select[3] = { UF_MODL_MID_POINT,  
                                UF_MODL_MID_POINT,  
                                UF_MODL_MID_POINT },  
            which_plane = 1;  
        tag_t  
            edge_tags[3],  
            dplanes[2];  
        edge_tags[0] = select_an_edge("select first edge");  
        edge_tags[1] = select_an_edge("select second edge");  
        edge_tags[2] = select_an_edge("select third edge");  
        UF_CALL(UF_MODL_create_relative_dplane(3, edge_tags, point_select,  
            which_plane, ref_point, "0", "0", &num_dplanes, dplanes));  
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

> 这段代码是基于NX的二次开发，主要实现了以下功能：
>
> 1. 错误处理机制：通过定义report_error函数，实现了错误码的打印，并在UF_CALL宏中调用该函数，用于输出函数执行错误时的文件名、行号、错误码等信息，便于调试。
> 2. 选择边线：通过select_an_edge函数，实现了根据提示选择模型中的边线，并返回边线的tag。使用了UF_UI接口。
> 3. 创建平面：通过do_it函数，实现了选择三条边线，并通过UF_MODL_create_relative_dplane函数，根据这三条边线创建一个平面。这里使用了边线的中点作为参考点，并且指定了创建的平面为第二个平面。
> 4. 用户函数入口：在ufusr函数中，首先调用UF_initialize函数初始化NX，然后调用do_it函数执行功能，最后调用UF_terminate函数结束NX。
> 5. 卸载函数：ufusr_ask_unload函数返回了立即卸载，表示用户函数在卸载时不需要进行任何清理工作。
>
> 综上所述，这段代码通过NX的UF和UI接口，实现了根据用户选择的三个边线创建一个平面的功能。
>
