### 【0687】create explosion 创建爆炸

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
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
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void ask_next_available_name(char *prefix, char *unique_name)  
    {  
        int  
            ii = 0;  
        tag_t  
            object = NULL_TAG;  
        do  
        {  
            ii++;  
            sprintf(unique_name, "%s%d", prefix, ii);  
            UF_CALL(UF_OBJ_cycle_by_name(unique_name, &object));  
            if (object == NULL_TAG) return;  
        } while (true);  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void ask_component_origin_and_matrix(tag_t component, double origin[3],  
        double csys_matrix[9])  
    {  
        double  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }  
    static void map_vector_to_component(tag_t component, double vector[3])  
    {  
        int  
            irc;  
        double  
            abs_mx[] = { 0,0,0, 1,0,0, 0,1,0 },  
            comp_mx[9] = { 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            matrix[9],  
            mapping_mx[12];  
        ask_component_origin_and_matrix(component, origin, matrix);  
        UF_VEC3_copy(matrix, &(comp_mx[3]));  
        UF_VEC3_copy(&(matrix[3]), &(comp_mx[6]));  
        FTN(uf5940)(abs_mx, comp_mx, mapping_mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vector, mapping_mx);  
    }  
    #include <uf_undo.h>  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            component,  
            explosion = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            work_view;  
        double  
            default_direction[3],  
            direction[3],  
            distance = 10,  
            origin[3],  
            matrix[9],  
            translate[3],  
            trans_mat[4][4];  
        char  
            unique_name[MAX_ENTITY_NAME_SIZE +1];  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_ASSEM_ask_view_explosion(work_view, &explosion));  
        if (explosion == NULL_TAG)  
        {  
            ask_next_available_name("Explosion ", unique_name);  
            UF_CALL(UF_ASSEM_create_explosion(part, unique_name, &explosion));  
            UF_CALL(UF_ASSEM_set_view_explosion(work_view, explosion));  
            UF_DISP_regenerate_view(work_view);  
        }  
        while (  
            (component = select_a_component("Component to Explode")) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_explosion_vector(component, default_direction));  
            ask_component_origin_and_matrix(component, origin, matrix);  
            display_conehead(origin, default_direction);  
            if (!specify_vector("Direction to Explode - Cancel to accept default",  
                origin, direction))  
                UF_VEC3_copy(default_direction, direction);  
            prompt_for_a_number("Explode", "Distance", &distance);  
            map_vector_to_component(component, direction);  
            UF_VEC3_scale(distance, direction, translate);  
            trans_mat[0][0] = 1;  
            trans_mat[0][1] = 0;  
            trans_mat[0][2] = 0;  
            trans_mat[0][3] = translate[0];  
            trans_mat[1][0] = 0;  
            trans_mat[1][1] = 1;  
            trans_mat[1][2] = 0;  
            trans_mat[1][3] = translate[1];  
            trans_mat[2][0] = 0;  
            trans_mat[2][1] = 0;  
            trans_mat[2][2] = 1;  
            trans_mat[2][3] = translate[2];  
            trans_mat[3][0] = 0;  
            trans_mat[3][1] = 0;  
            trans_mat[3][2] = 0;  
            trans_mat[3][3] = 1;  
            set_undo_mark("Explode component");  
            UF_CALL(UF_ASSEM_explode_component(explosion, component, trans_mat));  
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

> 根据代码的注释和函数名，这段代码实现了以下功能：
>
> 1. 定义了一些辅助函数，如ECHO用于打印日志，report_error用于报告错误，mask_for_components用于设置选择组件的掩码，select_a_component用于选择组件，specify_vector用于指定一个向量，prompt_for_a_number用于提示用户输入一个数字，ask_next_available_name用于生成一个唯一的名称，display_conehead用于显示锥形头部，ask_component_origin_and_matrix用于获取组件的坐标系信息，map_vector_to_component用于将一个向量映射到组件坐标系，set_undo_mark用于设置撤销标记。
> 2. 主函数do_it()实现了以下流程：获取当前工作视图和爆炸视图如果没有爆炸视图，则创建一个新的爆炸视图并设置到当前工作视图循环选择组件，为每个组件执行以下操作：获取组件的默认爆炸方向获取组件的坐标系信息提示用户指定爆炸方向，如果没有指定，则使用默认方向提示用户输入爆炸距离将爆炸方向映射到组件坐标系根据爆炸方向和距离计算平移向量根据平移向量构建变换矩阵设置撤销标记使用变换矩阵爆炸组件
> 3. 获取当前工作视图和爆炸视图
> 4. 如果没有爆炸视图，则创建一个新的爆炸视图并设置到当前工作视图
> 5. 循环选择组件，为每个组件执行以下操作：获取组件的默认爆炸方向获取组件的坐标系信息提示用户指定爆炸方向，如果没有指定，则使用默认方向提示用户输入爆炸距离将爆炸方向映射到组件坐标系根据爆炸方向和距离计算平移向量根据平移向量构建变换矩阵设置撤销标记使用变换矩阵爆炸组件
> 6. 获取组件的默认爆炸方向
> 7. 获取组件的坐标系信息
> 8. 提示用户指定爆炸方向，如果没有指定，则使用默认方向
> 9. 提示用户输入爆炸距离
> 10. 将爆炸方向映射到组件坐标系
> 11. 根据爆炸方向和距离计算平移向量
> 12. 根据平移向量构建变换矩阵
> 13. 设置撤销标记
> 14. 使用变换矩阵爆炸组件
> 15. ufusr是NX调用的主函数，用于初始化NX环境，执行do_it()函数，然后终止NX环境。
> 16. ufusr_ask_unload函数用于处理卸载用户程序时的清理工作。
>
> 综上，这段代码实现了NX组件的交互式爆炸功能。
>
