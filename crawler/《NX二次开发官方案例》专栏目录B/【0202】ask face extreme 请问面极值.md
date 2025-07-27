### 【0202】ask face extreme 请问面极值

#### 代码

```cpp
    /*HEAD ASK_FACE_EXTREME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志信息。 */  
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
    static logical prompt_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static void whatis(char *name, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        char  
            *feat_type;  
        printf("'%s' - tag %d is ", name, object);  
        if (object)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_feature_type)  
            {  
                UF_CALL(UF_MODL_ask_feat_type( object, &feat_type));  
                printf("a %s feature\n", feat_type);  
                UF_free(feat_type);  
            }  
            else  
            {  
                if (UF_ASSEM_is_occurrence(object))  
                    printf("an occurrence, ");  
                status = UF_OBJ_ask_status(object);  
                printf("type %d, subtype %d", type, subtype);  
                if ((type == UF_solid_type) &&  
                    (subtype == UF_solid_face_subtype) &&  
                    (status == UF_OBJ_ALIVE))  
                {  
                    UF_CALL(UF_MODL_ask_face_type(object, &type));  
                    printf(", face type %d", type);  
                }  
                printf(", status %d\n", status);  
            }  
        }  
        else  
            printf("a NULL_TAG\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            point,  
            subent = NULL_TAG;  
        double  
            cpc[3],  
            cps[3],  
            dir1[3] = { 0, 0, -1 },  
            dir2[3] = { -1, 0, 0 },  
            dir3[3] = { 0, 1, 0 },  
            dist;  
        while (((face = select_a_face("Select a face")) != NULL_TAG) &&  
                prompt_vector("Specify dir1", dir1) &&  
                prompt_vector("Specify dir2", dir2) &&  
                prompt_vector("Specify dir3", dir3))  
        {  
            if (subent) UF_CALL(UF_DISP_set_highlight(subent, FALSE));  
            UF_CALL(UF_MODL_ask_extreme(face, dir1, dir2, dir3, &subent, cpc));  
            printf("\npoint is at %f, %f, %f\n", cpc[0], cpc[1], cpc[2]);  
            whatis("subent", subent);  
            UF_CALL(UF_CURVE_create_point(cpc, &point));  
            if (subent)  
            {  
                UF_CALL(UF_DISP_set_highlight(subent, TRUE));  
                UF_CALL(UF_MODL_ask_minimum_dist( point, subent, TRUE, cpc,  
                    TRUE, cpc, &dist, cpc, cps));  
                printf("distance to subent is %f\n", dist);  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_minimum_dist( point, face, TRUE, cpc,  
                    TRUE, cpc, &dist, cpc, cps));  
                printf("distance to face is %f\n", dist);  
            }  
        }  
        if (subent) UF_CALL(UF_DISP_set_highlight(subent, FALSE));  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是查询曲面的极值点。代码的主要流程如下：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate函数来初始化和终止NX的二次开发环境。
> 2. 选择曲面：使用select_a_face函数，通过对话框让用户选择一个曲面实体。
> 3. 输入方向向量：通过prompt_vector函数提示用户输入三个方向向量dir1、dir2、dir3。
> 4. 查询极值点：使用UF_MODL_ask_extreme函数，根据输入的方向向量，查询曲面的极值点subent和坐标cpc。
> 5. 创建点：使用UF_CURVE_create_point函数，在极值点位置创建一个点。
> 6. 查询最小距离：使用UF_MODL_ask_minimum_dist函数，查询点到曲面或极值子实体的最小距离和位置cps。
> 7. 高亮显示：使用UF_DISP_set_highlight函数，高亮显示曲面和极值子实体。
> 8. 循环查询：通过while循环，让用户可以重复选择曲面和方向，查询不同曲面的极值点。
> 9. 错误处理：使用report_error函数，打印API调用失败的错误信息。
> 10. 卸载提示：提供ufusr_ask_unload函数，提示立即卸载二次开发程序。
>
> 这个示例代码展示了如何使用NX Open C++ API进行二次开发，通过查询曲面的极值点，为用户提供一个实用的曲面分析工具。
>
