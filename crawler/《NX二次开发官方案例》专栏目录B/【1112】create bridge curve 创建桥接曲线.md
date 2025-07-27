### 【1112】create bridge curve 创建桥接曲线

#### 代码

```cpp
    /*HEAD CREATE_BRIDGE_CURVE CCC UFUN */  
    /*This is a sample program, you would probably want to */  
    /* add a way of testing for, and setting the desired parm value  译:增加测试并设置期望参数值的方法 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数，用于在用户空间打印内核日志。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
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
            curves[2],  
            bridge_id;  
        double  
            parm[2];         
        int  
            rev_tgt[2];   
        parm[0]=1.0;  
        parm[1]=.0;  
        rev_tgt[0]=0;   
        rev_tgt[1]=0;   
        while ( ( (curves[0] = select_a_curve("Select first curve") ) != NULL_TAG) &&  
                ( (curves[1] = select_a_curve("Select second curve") ) != NULL_TAG)   
               )  
        {  
            UF_CALL(UF_CURVE_create_bridge_curve(UF_CURVE_TANGENT, curves, parm, rev_tgt, &bridge_id));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 包含必要的NX开发头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个选择曲线的函数select_a_curve，用于提示用户选择曲线，并返回曲线的tag。
> 4. 定义了一个创建桥接曲线的函数do_it，该函数会提示用户选择两个曲线，然后调用UF_CURVE_create_bridge_curve函数创建桥接曲线。
> 5. 定义了ufusr函数作为程序的入口，初始化NX环境，调用do_it函数实现主体功能，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于立即卸载用户程序。
>
> 该程序的主要功能是让用户选择两个曲线，然后创建一个桥接曲线连接这两个曲线。程序通过NX提供的UF_CURVE_create_bridge_curve函数实现桥接曲线的创建。
>
