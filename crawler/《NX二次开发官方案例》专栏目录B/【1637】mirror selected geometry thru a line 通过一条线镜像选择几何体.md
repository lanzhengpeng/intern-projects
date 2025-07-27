### 【1637】mirror selected geometry thru a line 通过一条线镜像选择几何体

#### 代码

```cpp
    /*HEAD MIRROR_SELECTED_GEOMETRY_THRU_A_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
        int  
            one = 1,  
            n = 0,  
            resp = 0,  
            two = 2,  
            zero = 0;  
        tag_t  
            *objects,  
            line;  
        double  
            matrix[16] = { 0.,0.,0.,0.,  0.,0.,0.,0.,  0.,0.,0.,0.,  0.,0.,0.,0. };  
        while (((n = select_objects("Select objects to mirror:", &objects)) > 0)  
               && ((line = select_a_line("Select line:")) != NULL_TAG) )  
        {          
            FTN(uf5946)( &line, matrix, &resp);  
            if( resp ) WRITE_D( resp );  
            FTN(uf5947)( matrix,objects,&n,&one,&zero,&two,NULL,NULL,&resp);  
            if( resp ) WRITE_D( resp );  
            if( n ) UF_free(objects);  
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

> 这段NX二次开发代码的主要功能是让用户选择一组对象和一个线，然后沿着这条线对这些对象进行镜像对称。具体介绍如下：
>
> 1. 错误报告函数：包含错误报告函数report_error，用于在函数执行失败时打印错误信息。
> 2. 选择对象函数：包含选择对象函数select_objects，用于让用户选择一组要镜像的对象。
> 3. 线选择掩码函数：包含线选择掩码函数mask_for_lines，用于设置选择对话框的掩码，只允许选择线对象。
> 4. 选择线对象函数：包含选择线对象函数select_a_line，用于让用户选择一个线对象作为镜像对称轴。
> 5. 镜像对称函数：包含两个函数uf5946和uf5947，分别用于定义镜像对称轴和进行镜像对称操作。
> 6. 主函数：在主函数ufusr中，使用循环调用select_objects和select_a_line，让用户不断选择对象和线，然后调用镜像对称函数进行镜像对称操作。
> 7. 卸载函数：包含卸载函数ufusr_ask_unload，用于在插件卸载时返回立即卸载标志。
>
> 总体来说，这段代码实现了交互式选择对象和线，然后沿着这条线对对象进行镜像对称的功能。
>
