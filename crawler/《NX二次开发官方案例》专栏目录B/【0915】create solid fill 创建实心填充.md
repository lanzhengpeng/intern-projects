### 【0915】create solid fill 创建实心填充

#### 代码

```cpp
    /*HEAD CREATE_SOLID_FILL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_circle_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc_in_a_view(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_arcs, NULL, &resp,  
            &object, cp, view));  
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
            mpi[100],  
            bnds[1] = { 1 };  
        tag_t  
            arc,  
            fill,  
            view;  
        double  
            mpr[70];  
        char  
            diameter[27],  
            radius[27];  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[92] = 10;  /* Solid fill 里海译:Solid fill - 实心填充 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        while ((arc = select_an_arc_in_a_view("Solid fill", &view)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_create_areafill(1, bnds, &arc, view, &fill));  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是在NX中创建实心填充。
>
> 1. 代码结构：代码首先包含NX提供的uf.h等头文件，并定义了UF_CALL宏用于错误处理。然后定义了report_error函数用于打印错误信息。接着定义了mask_for_arcs函数用于设置选择过滤条件，只选择圆弧。然后定义了select_an_arc_in_a_view函数用于在视图中选择一个圆弧。最后定义了do_it函数用于创建实心填充，以及ufusr函数作为主函数入口。
> 2. 功能流程：在主函数ufusr中，先初始化NX环境，然后调用do_it函数。在do_it中，先获取当前的绘图参数，然后设置实心填充的参数，接着循环选择圆弧并创建实心填充，直到用户取消选择。最后在ufusr中结束NX环境。
> 3. 主要功能点：该代码的主要功能点包括选择圆弧、设置实心填充参数、创建实心填充等。通过使用NX提供的UI和绘图函数，实现了实心填充的创建功能。
> 4. 代码规范：代码注释清晰，变量命名规范，函数结构合理，错误处理完善，遵循了良好的编程规范。
> 5. 适用性：该代码可作为NX二次开发中创建实心填充的参考示例，具有很好的学习和借鉴价值。
>
