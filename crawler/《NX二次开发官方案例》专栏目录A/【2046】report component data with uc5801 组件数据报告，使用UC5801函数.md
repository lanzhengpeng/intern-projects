### 【2046】report component data with uc5801 组件数据报告，使用UC5801函数

#### 代码

```cpp
    /*HEAD REPORT_COMPONENT_DATA_WITH_UC5801 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，可以翻译为：

注意：UF_print_syslog 是 V18 新增的功能，请只提供翻译，不要添加无关内容。 */  
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
    #define WRITE12F(X)  (write_double_array_to_listing_window(#X, X, 4, 3))  
    /* qq3123197280 */  
    static void write_double_array_to_listing_window(char *name, double *array,  
                                                    int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        UF_CALL(UF_UI_write_listing_window("\n"));  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp;  
        char  
            cname[MAX_ENTITY_NAME_SIZE+1],  
            pname[MAX_FSPEC_SIZE+1],  
            rname[MAX_ENTITY_NAME_SIZE+1];  
        double    
            matrix[13];  
        int   
            ctimes[4];  
        while ((comp = select_a_component("Select Component")) != NULL_TAG)  
        {  
            uc5801(comp, cname, pname, rname, matrix, ctimes);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_CALL(UF_UI_write_listing_window("component name: "));  
            }  
            WRITE(cname);  
            UF_CALL(UF_UI_write_listing_window("\nPart name: "));  
            WRITE(pname);  
            UF_CALL(UF_UI_write_listing_window("\n Refset Name:"));  
            WRITE(rname);  
            WRITE12F(matrix);  
            WRITEnD(4,ctimes);  
            UF_CALL(UF_UI_write_listing_window("\n"));  
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

> 这段代码是NX的二次开发代码，其主要功能是获取当前装配体中所有组件的信息，并输出到列表窗口。具体来说，代码主要实现以下功能：
>
> 1. 定义错误报告函数report_error，用于输出函数调用错误到系统日志和列表窗口。
> 2. 定义组件选择遮罩函数mask_for_components，用于在组件选择对话框中仅显示组件类型的对象。
> 3. 定义选择组件函数select_a_component，用于弹出组件选择对话框，并返回选择的组件Tag。
> 4. 定义数组输出到列表窗口的宏WRITE12F和WRITEnD，用于将双精度浮点数数组和整数数组格式化输出到列表窗口。
> 5. 定义核心功能函数do_it，循环调用select_a_component选择组件，然后调用uc5801函数获取组件信息，并输出到列表窗口。
> 6. 定义主函数ufusr，初始化NX环境后调用do_it函数，最后终止NX环境。
> 7. 定义卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中获取并输出装配体组件信息的功能，主要采用了NX提供的UI类函数来实现组件选择和数据显示。
>
