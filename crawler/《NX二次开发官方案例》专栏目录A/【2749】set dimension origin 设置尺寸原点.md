### 【2749】set dimension origin 设置尺寸原点

#### 代码

```cpp
    /*HEAD SET_DIMENSION_ORIGIN CCC UFUN */  
    #include <uf_drf.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18版本中是新增的。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t   dim1;  
        double  dim_origin[3];  
        while( (dim1 = select_a_dimension("Select dimension to relocate")) != NULL_TAG)  
        {  
            UF_CALL( UF_DRF_ask_origin( dim1, dim_origin));  
                printf("Old origin: %f, %f, %f\n", dim_origin[0], dim_origin[1], dim_origin[2]);  
            specify_point("Specify new location", dim_origin);  
            UF_CALL( UF_DRF_set_origin( dim1, dim_origin));  
                printf("New origin: %f, %f, %f\n", dim_origin[0], dim_origin[1], dim_origin[2]);  
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

> 这段代码是一个用于NX Open的UF函数，主要功能是让用户选择一个尺寸并重新设置其原点。具体来说，代码的主要流程如下：
>
> 1. 定义了一些宏和函数，用于错误处理和日志输出。
> 2. 实现了一个函数mask_for_dimensions，用于设置选择尺寸的类型和子类型。
> 3. 实现了一个函数select_a_dimension，用于显示一个对话框，让用户选择一个尺寸。
> 4. 实现了一个函数specify_point，用于提示用户输入一个新的点坐标。
> 5. 主函数do_it中，使用一个循环让用户不断选择尺寸，然后获取和设置尺寸的原点。
> 6. 在ufusr函数中初始化和终止NX Open，并调用do_it函数。
> 7. ufusr_ask_unload函数用于卸载用户函数。
>
> 总的来说，这是一个用于选择尺寸并重新设置其原点的NX Open二次开发代码示例。
>
