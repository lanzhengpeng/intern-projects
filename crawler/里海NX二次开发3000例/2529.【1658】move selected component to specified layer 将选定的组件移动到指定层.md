### 【1658】move selected component to specified layer 将选定的组件移动到指定层

#### 代码

```cpp
    /*HEAD MOVE_SELECTED_COMPONENT_TO_SPECIFIED_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    /* qq3123197280 */  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG;  
        int  
            new_layer = 0;  
        if(  
            ((comp = select_a_component("Pick Component to Move:" )) != NULL_TAG )  
            && (prompt_for_a_layer("New Layer", "Layer", &new_layer))  
          )  
        {  
            UF_CALL(UF_OBJ_set_layer(comp, new_layer));   
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

> 这段代码是一个NX Open C++程序，用于实现以下功能：
>
> 1. 错误报告函数：report_error用于报告函数调用失败时的错误信息，包括文件名、行号、调用函数名和错误码。
> 2. 选择组件：select_a_component函数用于弹出一个对话框，让用户选择一个组件，并返回组件的tag。
> 3. 提示输入层号：prompt_for_a_layer函数用于弹出一个对话框，提示用户输入新的层号，并进行有效性检查。
> 4. 移动组件到指定层：do_it函数用于选择一个组件，并提示用户输入新的层号，然后调用UF_OBJ_set_layer函数将组件移动到指定层。
> 5. 程序入口点：ufusr是程序的入口点，用于初始化NX Open，调用do_it函数执行功能，并在结束时关闭NX Open。
> 6. 卸载提示：ufusr_ask_unload用于提示立即卸载二次开发程序。
>
> 总体来说，这段代码的主要功能是实现将NX装配中的组件移动到指定的层。通过弹出的对话框，让用户选择组件和输入层号，然后调用NX Open的API函数来实现组件的移动。
>
