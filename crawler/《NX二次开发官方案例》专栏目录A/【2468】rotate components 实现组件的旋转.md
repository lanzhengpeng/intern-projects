### 【2468】rotate components 实现组件的旋转

#### 代码

```cpp
    /*HEAD ROTATE_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
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
    static void rotate_component(tag_t comp, double vec[3], double pnt[3],  
        double angle)  
    {  
        int  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            csys,  
            instance,  
            mx;  
        double  
            instance_origin[3],  
            instance_matrix[9],  
            instance_transform[4][4],  
            rotation[16];  
        char  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        instance = UF_ASSEM_ask_inst_of_part_occ(comp);  
        UF_CALL( UF_ASSEM_ask_component_data( instance, part_name, refset_name,  
            instance_name, instance_origin, instance_matrix, instance_transform));  
        UF_MTX3_ortho_normalize(instance_matrix);  
        UF_CALL(UF_CSYS_create_matrix(instance_matrix, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(instance_origin, mx, &csys));  
        FTN(uf5945)(pnt, vec, &angle, rotation, &resp);  
        FTN(uf5947)(rotation, &csys, &one, &one, &zero, &two, NULL, NULL, &resp);  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &mx, instance_origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(mx, instance_matrix));  
        UF_CALL( UF_ASSEM_reposition_instance(instance, instance_origin,  
            instance_matrix));  
    }  
    static void do_it(void)  
    {  
        double  
            angle = 90,  
            pnt[3],  
            vec[3] = { 1, 0, 0 };  
        tag_t  
            comp;  
        while (((comp = select_a_component("Select a component")) != NULL_TAG) &&  
                (specify_vector("Rotate about", vec, pnt)) &&  
                (prompt_for_a_number("Rotation angle", "degrees", &angle)))  
            rotate_component(comp, vec, pnt, angle);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的UF程序，用于选择一个组件并绕指定的点旋转指定的角度。
>
> 主要函数包括：
>
> 1. mask_for_components()：设置选择过滤器，只允许选择组件类型的对象。
> 2. select_a_component()：弹出一个对话框，让用户选择一个组件，并返回组件的tag。
> 3. specify_vector()：弹出一个对话框，让用户指定一个向量。
> 4. prompt_for_a_number()：弹出一个对话框，让用户输入一个数字。
> 5. rotate_component()：绕指定的点和向量旋转组件指定的角度。首先获取组件的实例信息，创建一个临时坐标系，计算旋转矩阵，应用旋转，然后更新组件的位置和方向。
> 6. do_it()：循环调用select_a_component、specify_vector和prompt_for_a_number，获取组件、旋转轴和角度，然后调用rotate_component进行旋转。
> 7. ufusr()：程序的入口点。初始化NX Open，调用do_it，然后终止NX Open。
> 8. ufusr_ask_unload()：询问卸载函数，返回立即卸载。
>
> 该程序通过对话框交互式地选择组件、指定旋转轴和角度，实现了对组件的旋转功能。
>
