### 【0713】create hollow 创建空腔

#### 代码

```cpp
    /*HEAD CREATE_HOLLOW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以翻译为：

注意：UF_print_syslog 是 V18 中新增的。 */  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            hollow;  
        double  
            thickness = 0.02,  
            tolerance;  
        uf_list_p_t  
            faces;  
        char  
            parm[15];  
        UF_MODL_ask_distance_tolerance(&tolerance);  
        if (((face = select_a_face("Select face to pierce")) != NULL_TAG) &&  
            prompt_for_a_number("Enter hollow parameter", "thickness", &thickness))  
        {  
            sprintf(parm, "%f", thickness);  
            if (thickness < tolerance)  
            {  
                UF_CALL(UF_MODL_create_variable_hollow(thickness/10.0, parm, 1,  
                    &face, 0, NULL, 0, NULL, NULL, &hollow));  
            }  
            else  
            {  
                UF_CALL(UF_MODL_create_list(&faces));  
                UF_CALL(UF_MODL_put_list_item(faces, face));  
                UF_CALL(UF_MODL_create_hollow(parm, faces, &hollow));  
                UF_CALL(UF_MODL_delete_list(&faces));  
            }  
            ECHO(hollow);  
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

> 这段代码是用于NX二次开发的UF函数，主要功能是创建一个空心体。
>
> 代码的主要逻辑如下：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 2. 定义了选择面的函数select_a_face，用于让用户选择一个面作为创建空心的基础面。
> 3. 定义了提示用户输入参数的函数prompt_for_a_number，用于提示用户输入空心的厚度等参数。
> 4. 在主函数do_it中，先让用户选择一个面，然后提示用户输入空心的厚度参数。根据厚度与NX系统距离公差的关系，选择使用UF_MODL_create_variable_hollow或UF_MODL_create_hollow函数来创建空心。
> 5. 定义了ufusr函数作为程序的入口点，在初始化NX环境后调用do_it函数，最后结束NX环境。
> 6. 定义了ufusr_ask_unload函数，用于让NX在卸载用户函数时立即卸载，而不是等待。
>
> 综上所述，这段代码实现了在NX中创建一个空心体的功能，通过UF函数实现用户交互和模型操作，展现了NX二次开发的基本流程。
>
