### 【0808】create daxis using face 使用面创建基准轴

#### 代码

```cpp
    /*HEAD CREATE_DAXIS_USING_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog在V18版本中是新增的。

只回答翻译，不要添加无关内容。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            junk[3] = { 0,0,0 };  
        tag_t  
            daxis,  
            face;  
        while ((face = select_a_face("Select face for datum axis")) != NULL_TAG)  
            UF_CALL(UF_MODL_create_relative_daxis(1, &face, junk, &daxis));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API函数失败时打印错误信息，方便调试。
> 2. 选择面：定义了一个函数mask_for_faces，用于在对话框中选择一个面，并设置选择掩码为只选择实体面。
> 3. 选择一个面：定义了一个函数select_a_face，用于弹出一个对话框，让用户选择一个面，并返回该面的tag。
> 4. 创建基准轴：定义了一个函数do_it，用于循环调用select_a_face，让用户选择面，然后调用NX API函数UF_MODL_create_relative_daxis，在每个面上创建一个基准轴，直到用户选择取消。
> 5. 主函数：定义了ufusr主函数，首先初始化NX环境，然后调用do_it函数，最后在结束时终止NX环境。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载，表示NX关闭时可以立即卸载此代码。
>
> 综上所述，这段代码实现了选择面并创建基准轴的功能，用户可以通过选择不同的面来创建多个基准轴，直到取消选择为止。
>
