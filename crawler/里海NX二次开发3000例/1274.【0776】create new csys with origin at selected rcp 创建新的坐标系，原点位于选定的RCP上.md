### 【0776】create new csys with origin at selected rcp 创建新的坐标系，原点位于选定的RCP上

#### 代码

```cpp
    /*HEAD CREATE_NEW_CSYS_WITH_ORIGIN_AT_SELECTED_RCP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_route.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* 里海 */  
    static int mask_for_rcps(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_control_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_an_rcp(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an RCP", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_rcps, NULL, &resp,  
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
        tag_t  
            rcp = NULL_TAG,  
            mx = NULL_TAG,  
            wcs = NULL_TAG,  
            new_csys = NULL_TAG;  
        double  
            wcs_origin[3] = { 0. ,0., 0. },  
            rcp_origin[3] = { 0. ,0., 0. };  
        rcp = select_an_rcp( "Pick RCP:" );  
        if( NULL_TAG == rcp ) return;  
        UF_CALL( UF_ROUTE_ask_rcp_position( rcp, rcp_origin ));  
        UF_CALL( UF_CSYS_ask_wcs( &wcs ));  
        UF_CALL( UF_CSYS_ask_csys_info( wcs, &mx, wcs_origin ));  
        UF_CALL( UF_CSYS_create_csys( rcp_origin, mx, &new_csys ));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个选择函数mask_for_rcps，用于在对话框中选择一个RCP（Route Control Point）类型的对象。
> 3. 定义了一个选择RCP的函数select_an_rcp，用于弹出一个对话框，让用户选择一个RCP对象，并返回其Tag。
> 4. 定义了一个do_it函数，用于获取选择的RCP的坐标，获取当前工作坐标系 WCS 的信息，然后以 RCP 的坐标为中心，创建一个新的坐标系。
> 5. 定义了ufusr函数，这是 NX Open 的入口函数，用于初始化和执行do_it函数。
> 6. 定义了ufusr_ask_unload函数，用于卸载动态库。
>
> 总的来说，这段代码实现了以一个选择的RCP为中心点，创建一个新坐标系的功能。通过选择一个RCP，获取其坐标，然后以该坐标为中心，创建一个新的坐标系。这为后续的建模和编程提供了方便。
>
