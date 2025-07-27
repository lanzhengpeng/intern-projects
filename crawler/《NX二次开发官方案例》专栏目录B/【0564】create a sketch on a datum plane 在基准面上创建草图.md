### 【0564】create a sketch on a datum plane 在基准面上创建草图

#### 代码

```cpp
    /*HEAD CREATE_A_SKETCH_ON_DATUM_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，UF_print_syslog在V18版本中是一个新增的功能。 */  
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
    static int mask_for_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_datum_plane_type, 0, 0 },  
                        { UF_datum_axis_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane or axis",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datums, NULL, &resp,  
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
            datum,  
            feat,  
            h_axis,  
            objects[2] = { NULL_TAG, NULL_TAG },  
            sketch_id;  
        double  
            origin[3],  
            norm[3],  
            pt2[3];  
        char  
            buf[133],  
            *offdst,  
            *ang,  
            name[MAX_ENTITY_NAME_SIZE+1] = { "DATUM_SKETCH" };  
        int  
            reference[2] =  { 1, 1 };   
        while ((datum = select_a_datum("Select datum")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(datum, &feat));  
            UF_MODL_ask_datum_plane_parms( feat, origin, norm, &offdst, &ang );  
            pt2[0] = origin[0];  
            pt2[1] = origin[1];  
            pt2[2] = origin[2];  
            if(fabs(norm[0]) < .001)   
                pt2[0] = origin[0] + 1;  
            else  
            {  
                if(fabs(norm[1]) < .001)   
                    pt2[1] = origin[1] + 1;  
                else  
                    pt2[2] = origin[2] + 1;  
            }  
            UF_MODL_create_fixed_daxis( origin, pt2, &h_axis);  
            objects[0] = datum;  
            objects[1] = h_axis;  
            UF_CALL(UF_SKET_initialize_sketch(name, &sketch_id));  
            UF_CALL(UF_SKET_create_sketch(name, 1, NULL, objects, reference, 1, &sketch_id));  
            UF_CALL(UF_SKET_terminate_sketch());  
            UF_free(offdst);  
            UF_free(ang);  
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

> 这段代码是用于NX的二次开发，主要功能是在一个基准平面上创建一个草图。代码的主要组成部分如下：
>
> 1. 头文件包含：包含了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：用于在发生错误时输出错误信息，包括文件名、行号、调用函数和错误码等。
> 3. 选择基准面和轴的函数：通过对话框让用户选择一个基准平面或轴，并返回选择的基准面或轴的tag。
> 4. 创建草图的函数：首先获取基准面的参数，然后创建一个垂直于该基准面的固定轴，接着初始化草图环境，创建草图，并结束草图环境。
> 5. 主函数：初始化NX API，调用创建草图函数，结束NX API。
> 6. 卸载函数：在用户请求卸载时立即卸载。
>
> 总体来说，这段代码实现了在用户选择的基准平面上创建一个草图的功能，并且具有较好的错误处理和用户交互。
>
