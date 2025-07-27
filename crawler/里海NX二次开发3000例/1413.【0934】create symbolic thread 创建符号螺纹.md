### 【0934】create symbolic thread 创建符号螺纹

#### 代码

```cpp
    /*HEAD CREATE_SYMBOLIC_THREAD CCC UFUN */  
    /*  
    This program creates a 5/16-18 UNC symbolic thread.  
    It requires that you have a solid with a .257" diameter hole,  
    which starts in a planar face.   
    The direction of the hole is expected to be in the negative Z axis.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_types.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
    /* qq3123197280 */  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
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
        tag_t  
            plane = NULL_TAG,  
            sym_thread = NULL_TAG,  
            my_face = NULL_TAG;  
        UF_MODL_symb_thread_data_t  
            thread;  
        while(  
             ((my_face = select_a_face("Pick Cylindrical Face:")) != NULL_TAG) &&  
              (plane = select_a_face("Select Face Where Hole Starts:")) != NULL_TAG  
            )  
        {  
            thread.cyl_face = my_face;  
            thread.start_face = plane;  
            thread.major_dia = "0.3125";  
            thread.tapped_dia = "0.2674";  
            thread.minor_dia = "0.2464";  
            thread.axis_direction[0]=0;  
            thread.axis_direction[1]=0;  
            thread.axis_direction[2]=-1;  
            thread.include_instances = UF_MODL_INCL_INSTANCES;  
            thread.rotation = UF_MODL_RIGHT_HAND;  
            thread.length_flag = UF_MODL_FIXED_LENGTH;  
            thread.tapered = UF_MODL_NON_TAPERED;  
            thread.num_starts = 1;  
            thread.length= ".75";  
            thread.form = "Unified";  
            thread.method = "CUT";  
            thread.callout = "5/16-18_UNC";  
            UF_CALL(UF_MODL_create_symb_thread(&thread, &sym_thread));  
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

> 这段代码是一个用于创建5/16-18 UNC螺纹的NX二次开发程序。它主要包含以下部分：
>
> 1. 包含头文件：引入了NX Open的UF、UF_modl、UF_modl_types、UF_ui等模块的头文件。
> 2. 错误处理函数：用于在函数调用失败时，打印错误信息到系统日志。
> 3. 选择面函数：用于设置选择面时的掩码，并选择一个面。
> 4. 创建螺纹函数：用户选择一个圆柱面和一个起始面，然后设置螺纹的参数，并调用UF_MODL_create_symb_thread函数创建螺纹。
> 5. 主函数：初始化NX，调用创建螺纹函数，然后终止NX。
> 6. 卸载函数：当卸载此程序时，立即卸载。
>
> 总体来说，这是一个典型的NX二次开发程序，通过设置参数并调用NX提供的API函数，实现了创建螺纹的功能。程序采用了面向对象的思想，将创建螺纹的参数封装在一个结构体中，然后通过调用函数实现功能，代码结构清晰、模块化。
>
