### 【0652】create crosshatch 创建网状

#### 代码

```cpp
    /*HEAD CREATE_CROSSHATCH CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
    /* 里海 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
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
            xhatch_tag,  
            view_tag,  
            obj_tag[1];  
        int  
            obj_bnd[1] = { 1 };  
        char  
            view_name[30];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        obj_tag[0] = select_an_arc("Select a circle");  
        UF_CALL(uc1653(view_name));  
        printf("view name is : %s\n", view_name);  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
        UF_CALL(UF_DRF_create_crosshatch(1,obj_bnd,obj_tag,view_tag,&xhatch_tag));  
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

> 这是段NX Open C++开发的二次开发代码，主要功能是创建圆形轮廓的交叉线。以下是代码的主要功能介绍：
>
> 1. 包含头文件：代码包含了NX Open C++开发所需的头文件，如uf.h、uf_view.h等。
> 2. 错误报告函数：定义了report_error函数，用于打印错误信息，包括文件名、行号、调用函数和错误码。
> 3. 选择圆形轮廓：定义了mask_for_arcs函数，用于设置选择过滤条件，只选择圆形轮廓。定义了select_an_arc函数，用于打开选择对话框，让用户选择一个圆形轮廓。
> 4. 创建交叉线：在do_it函数中，首先选择一个圆形轮廓，然后获取当前视图的视图名，接着获取视图的标签，最后调用UF_DRF_create_crosshatch函数创建圆形轮廓的交叉线。
> 5. UFusr入口函数：定义了ufusr函数作为程序的入口，用于初始化NX系统，执行do_it函数，最后终止NX系统。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于返回程序卸载方式，这里设置为立即卸载。
>
> 综上，该代码实现了创建圆形轮廓交叉线的主要功能，采用了NX Open C++进行开发。
>
