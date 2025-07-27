### 【0669】create double offset chamfer 创建双偏移倒角

#### 代码

```cpp
    /*HEAD CREATE_DOUBLE_OFFSET_CHAMFER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的说明，UF_print_syslog是V18版本新增的函数。文档要求只提供翻译，不添加其他内容。因此，翻译为：UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            double_offset = 2,  
            count = 0;  
        tag_t  
            edge = NULL_TAG,  
            face = NULL_TAG,  
            chamfer = NULL_TAG;  
        char  
            off_1_str[20] = { "" },  
            off_2_str[20] = { "" },  
            theta[20] = { "0.0" },  
            msg[132] = { "" };  
        double  
            off_1_num = 0.,  
            off_2_num = 0.;  
        uf_list_p_t  
            edge_list,  
            face_list;  
        while(( edge = select_an_edge("Select an Edge:" )) != NULL_TAG )  
        {      
            UF_MODL_create_list( &edge_list );   
            UF_CALL( UF_MODL_put_list_item( edge_list, edge ));  
            UF_CALL( UF_MODL_ask_edge_faces( edge, &face_list ));  
            UF_CALL(UF_MODL_ask_list_count( face_list, &count ));  
            if( count != 2) break; /* count = 1 for a sheet body 译:翻译为：一个表体的计数为1。 */  
            UF_CALL( UF_MODL_ask_list_item( face_list, 0, &face ));  
            prompt_for_two_numbers( "Enter offset values", "First", "Second",  
                                                       &off_1_num, &off_2_num );  
            UF_CALL(UF_DISP_set_highlight( face, 1 ));   
            /* query to see if face order is acceptable 译:根据查询，判断面部顺序是否可以接受。 */   
            sprintf( msg, "Offset %f from this face?", off_1_num );  
            if( ask_yes_or_no("Verify Offset", msg))  
            {                                           /* offsets the first value   
                                                              from the first face */  
                sprintf( off_1_str, "%f", off_1_num );  
                sprintf( off_2_str, "%f", off_2_num );  
            }  
            else                  /* offsets the first value from the second face 译:将第一个值从第二个值中偏移。 */  
            {     
                sprintf( off_1_str, "%f", off_2_num );  
                sprintf( off_2_str, "%f", off_1_num );  
            }  
            UF_CALL( UF_MODL_create_chamfer( double_offset, off_1_str,   
                               off_2_str, theta, edge_list, &chamfer ));   
            UF_CALL( UF_MODL_delete_list( &edge_list ));  
            UF_CALL( UF_MODL_delete_list( &face_list ));  
        }    
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
> 1. 错误报告函数：定义了一个名为report_error的函数，用于在出错时打印错误信息和调用信息，以便于调试。
> 2. 选择边：定义了mask_for_edges和select_an_edge两个函数，用于在图形窗口中选择边。
> 3. 提示输入：定义了prompt_for_two_numbers和ask_yes_or_no两个函数，用于提示用户输入两个数值和进行Yes/No确认。
> 4. 偏移倒角：do_it函数是核心逻辑，实现了选择边、获取相邻面、提示输入偏移量，并调用NX API创建偏移倒角的功能。
> 5. 二次开发入口：定义了ufusr函数作为二次开发的入口，用于初始化NX系统、执行核心逻辑和终止NX系统。
> 6. 卸载询问：定义了ufusr_ask_unload函数，当用户卸载该应用程序时，询问是否立即卸载。
>
> 总体来说，该代码通过NX提供的API，实现了选择边、创建偏移倒角的功能，并通过对话框提示用户进行交互，是一个典型的NX二次开发示例。
>
