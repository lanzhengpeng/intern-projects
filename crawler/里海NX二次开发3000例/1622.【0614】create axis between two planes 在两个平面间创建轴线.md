### 【0614】create axis between two planes 在两个平面间创建轴线

#### 代码

```cpp
    /*HEAD CREATE_AXIS_BETWEEN_TWO_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

UF_print_syslog是V18版本新增的，请只回答译文，不要添加其他内容。 */  
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
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
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
        tag_t   daxis_tag, planes[2];  
        int     pt_select[3];  
        planes[0] = select_a_datum_plane("Select first plane");  
        planes[1] = select_a_datum_plane("Select second plane");  
        UF_CALL( UF_MODL_create_relative_daxis( 2, planes, pt_select, &daxis_tag));  
            printf("Tag of axis created: %d\n", daxis_tag);  
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

> 这段代码是一个NX的二次开发用户函数(ufusr)，用于在两个平面之间创建一个相对的基准轴。以下是代码的主要内容和功能：
>
> 1. 头文件和宏定义：代码包含了必要的NX头文件，并定义了一个宏UF_CALL用于检查UF函数的返回码，并在出错时打印错误信息。
> 2. 错误报告函数：定义了一个report_error函数，用于在UF函数调用出错时打印错误信息。
> 3. 选择面过滤函数：定义了一个mask_for_datum_planes函数，用于在选择基准面时过滤出基准面类型。
> 4. 选择基准面函数：定义了一个select_a_datum_plane函数，用于弹出一个对话框让用户选择一个基准面，并返回基准面的tag。
> 5. 创建基准轴函数：定义了一个do_it函数，用于先选择两个基准面，然后调用UF_MODL_create_relative_daxis函数在这两个面之间创建一个相对的基准轴，并打印出新基准轴的tag。
> 6. ufusr主函数：这是用户函数的入口，首先调用UF_initialize初始化NX环境，然后调用do_it函数执行主要逻辑，最后调用UF_terminate终止NX环境。
> 7. 卸载函数：定义了一个ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该用户函数。
>
> 总体来说，这段代码实现了在两个基准面之间创建一个相对基准轴的功能。用户可以通过选择两个面，然后程序会自动创建一个与这两个面相关的基准轴。
>
