### 【2179】report leader side 报告引线方向

#### 代码

```cpp
    /*HEAD REPORT_LEADER_SIDE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。该函数用于打印系统日志。 */  
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
    /* qq3123197280 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            object;  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((object = select_a_dim_or_draft("Select object")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            switch(mpi[1])  
            {  
                case 1:   
                uc1601("The leader is from the Left", TRUE);  
                break;  
                case 2:   
                uc1601("The leader is from the Right", TRUE);  
                break;  
                default:   
                uc1601("No Leader side found", TRUE);  
                break;  
            }  
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

> 这段代码是NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时，打印错误信息到系统日志和控制台窗口。
> 2. 定义了一个选择过滤函数mask_for_dim_or_draft，用于在用户选择对象时，过滤只选择尺寸或草图对象。
> 3. 定义了一个选择对象函数select_a_dim_or_draft，弹出一个选择对话框，让用户选择一个尺寸或草图对象，并返回其tag。
> 4. 定义了一个处理函数do_it，循环调用select_a_dim_or_draft选择对象，然后调用UF_DRF_ask_object_preferences获取对象参数，判断尺寸的引线方向，并打印到控制台。
> 5. 定义了ufusr主函数，初始化UF环境，调用do_it函数处理，然后终止UF环境。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户自定义程序。
>
> 整体来说，这是一个简单的交互式二次开发示例，通过选择尺寸对象并获取其参数，实现了判断尺寸引线方向的功能。代码结构清晰，注释详细，便于理解和扩展。
>
