### 【1638】mirror selected geometry thru a plane 镜像选择的几何体通过一个平面

#### 代码

```cpp
    /*HEAD MIRROR_SELECTED_GEOMETRY_THRU_A_PLANE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，它用于在控制台上打印系统日志。 */  
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
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,  
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
            *objects = NULL,  
            plane = NULL_TAG;  
        double  
            matrix[16] = { 0.,0.,0.,0.,  0.,0.,0.,0.,  0.,0.,0.,0.,  0.,0.,0.,0. };  
        while (((n = select_objects("Select objects to mirror:", &objects)) > 0)  
               && ((plane = select_a_plane("Select plane:")) != NULL_TAG) )  
        {          
            FTN(uf5946)( &plane, matrix, &resp);  
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

> 这段NX Open C++代码用于执行“通过平面镜像几何体”的功能。
>
> 主要功能包括：
>
> 1. 初始化NX系统，并调用do_it()函数执行主体功能。
> 2. do_it()函数使用选择对话框让用户选择要镜像的几何体，并使用选择对话框让用户选择一个平面。
> 3. 调用UF函数计算镜像变换矩阵，并镜像几何体。
> 4. 重复执行上述过程，直到用户没有选择任何几何体。
> 5. 释放内存，并终止NX系统。
> 6. 提供了错误处理和日志打印功能。
> 7. 使用NX提供的UI函数来实现用户交互。
> 8. 使用了NX提供的几何变换函数来实现镜像操作。
> 9. 遵循了NX Open C++编程规范。
> 10. 提供了卸载函数。
>
> 整体来看，这段代码实现了通过平面镜像几何体的基本功能，同时考虑了错误处理和用户交互，是一个比较完整的NX Open C++二次开发示例。
>
