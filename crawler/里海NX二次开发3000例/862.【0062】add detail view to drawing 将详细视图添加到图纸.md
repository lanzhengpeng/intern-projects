### 【0062】add detail view to drawing 将详细视图添加到图纸

#### 代码

```cpp
    /*HEAD ADD_DETAIL_VIEW_TO_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <uf_draw.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译如下：

注：UF_print_syslog是V18版本新增的。 */  
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
    static void do_it(void)  
    {  
        const int   E_SIZE = 5;  
        tag_t       drawing;  
        tag_t       view_tag = NULL_TAG;  
        tag_t       detail_view_tag;  
        char        view_name[30]="";  
        double      size[2];  
        double      lower_left_corner[2] = { 4.0, 4.0};  
        double      upper_right_corner[2]= { 12.0, 12.0};  
        double      view_scale = 2, detail_ref_pt[2] = { 20.0, 20.0};  
        double      ref_pt[2] = { 4.0, 4.0};  
        /* Create a new drawing 译:创建新绘图 */  
        UF_CALL( uc6478( "SHEET1", 1, E_SIZE, size));  
        /* Add view to drawing 译:将视图添加到图纸 */  
        UF_CALL( uc6481( "SHEET1", "TOP", ref_pt, 1));  
        UF_CALL( UF_DRAW_ask_current_drawing( &drawing));  
        /* Cycle views on drawing 译:循环视图在绘图中的翻译为：Cycle views on drawing */  
        UF_CALL( uc6499( "SHEET1", view_name));  
        /* Get view tag from view name 译:获取视图名称的视图标签 */  
        UF_CALL( UF_OBJ_cycle_by_name( view_name, &view_tag));  
        UF_CALL( UF_DRAW_add_detail_view( drawing, view_tag, lower_left_corner,  
                upper_right_corner, view_scale, detail_ref_pt, &detail_view_tag));  
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

> 根据代码内容，该段代码为NX Open C++ API实现的NX二次开发代码，主要功能是向NX绘图添加详细视图。具体步骤如下：
>
> 1. 定义了错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了主要功能函数do_it，其中：使用UF_DRAW_ask_current_drawing获取当前绘图；通过UF_OBJ_cycle_by_name获取视图标签；使用UF_DRAW_add_detail_view向绘图添加详细视图。
> 3. 使用UF_DRAW_ask_current_drawing获取当前绘图；
> 4. 通过UF_OBJ_cycle_by_name获取视图标签；
> 5. 使用UF_DRAW_add_detail_view向绘图添加详细视图。
> 6. ufusr函数为NX Open API的回调函数，其中调用do_it函数执行添加详细视图操作。
> 7. ufusr_ask_unload函数用于卸载二次开发应用程序。
>
> 总体来说，这段代码实现了向NX绘图添加详细视图的功能，并包含了错误处理和应用程序管理。
>
