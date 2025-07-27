### 【0580】create and modify equilateral triangles using uistyler real scale 创建和修改等边三角形，使用UIStyler实数比例尺

#### 代码

```cpp
    /*HEAD CREATE_AND_MODIFY_EQUILATERAL_TRIANGLES_USING_UISTYLER_REAL_SCALE CCC UFUN */  
    /*  This program demonstrates modifying wireframe geometry based on the  
        value specified using a real scale UIStyler object.  
        To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Set the Dialog Title to "Create/Edit triangles"  
        - Set the Cue to "Indicate center point or select triangle to modify"  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC_"const_cb"  
        - Enable selection - Single Select, not Deselect, and Single Position  
        - Add a Real Scale  
        - Set the Initial Real Value to 1.0  
        - Set the Scale Limits 0.1 - 10.0 (or larger - but min must be > 0)  
        - Specify its Drag callback as "size_cb"  
        - Specify its Value Changed callback as "size_cb"  
        - Save Dialog As "triangle"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_group.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_mtx.h>  
    #define GTAC_CB_COUNT 4  
    #define GTAC_SCALE_REAL_0              ("SCALE_REAL_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的说明，UF_print_syslog是V18版本新增的函数。该函数用于在系统日志中打印信息。 */  
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
    struct TRI_data_s  
    {  
        int  
            dialog_id;  
        tag_t  
            csys,  
            group;  
        double  
            size,  
            origin[3];  
    };  
    typedef struct TRI_data_s TRI_data_t, *TRI_data_p_t;  
    static void set_highlight_group(tag_t group, int flag)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *objects;  
        UF_CALL(UF_GROUP_ask_group_data(group, &objects, &n));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        if (n > 0) UF_free(objects);  
    }  
    static void map_csys2abs(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void find_size_and_origin(TRI_data_p_t data)  
    {  
        int  
            n;  
        tag_t  
            matrix,  
            *old_sides;  
        double  
            mtx[9],  
            x_vec[3],  
            y_vec[3];  
        UF_CURVE_line_t  
            line_data;  
        UF_STYLER_item_value_type_t  
            value;  
        UF_CALL(UF_GROUP_ask_group_data(data->group, &old_sides, &n));  
        UF_CALL(UF_CURVE_ask_line_data(old_sides[0], &line_data));  
        UF_VEC3_distance(line_data.start_point, line_data.end_point, &data->size);  
        UF_VEC3_copy(line_data.start_point, data->origin);  
        UF_VEC3_sub(line_data.end_point, line_data.start_point, x_vec);  
        UF_CALL(UF_CURVE_ask_line_data(old_sides[1], &line_data));  
        UF_VEC3_sub(line_data.end_point, line_data.start_point, y_vec);  
        UF_CALL(UF_MTX3_initialize(x_vec, y_vec, mtx));  
        UF_CALL(UF_CSYS_create_matrix(mtx, &matrix));  
        UF_CALL(UF_CSYS_create_temp_csys(data->origin, matrix, &data->csys));  
        value.item_attr = UF_STYLER_VALUE;  
        value.item_id = GTAC_SCALE_REAL_0;  
        value.value.real = data->size;  
        value.count = 1;  
        UF_CALL(UF_STYLER_set_value(data->dialog_id, &value));  
    }  
    static void create_edit_triangle(TRI_data_p_t data)  
    {  
        int  
            n;  
        tag_t  
            new_sides[3],  
            *old_sides;  
        double  
            corner2[3],  
            corner3[3],  
            to2[3] = { 0,0,0 },  
            to3[3] = { 0,0,0 };  
        UF_CURVE_line_t  
            line_data = {{0,0,0}, {1,0,0}};  
        to2[0] = data->size;  
        to3[0] = data->size / 2;  
        to3[1] = sqrt(0.75 * data->size * data->size);  
        map_csys2abs(data->csys, to2);  
        map_csys2abs(data->csys, to3);  
        UF_VEC3_add(data->origin, to2, corner2);  
        UF_VEC3_add(data->origin, to3, corner3);  
        if (data->group == NULL_TAG)  
        {  
            UF_VEC3_copy(data->origin, line_data.start_point);  
            UF_VEC3_copy(corner2, line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &new_sides[0]));  
            UF_VEC3_copy(corner2, line_data.start_point);  
            UF_VEC3_copy(corner3, line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &new_sides[1]));  
            UF_VEC3_copy(corner3, line_data.start_point);  
            UF_VEC3_copy(data->origin, line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &new_sides[2]));  
            UF_CALL(UF_GROUP_create_group(new_sides, 3, &data->group));  
        }  
        else  
        {  
            UF_CALL(UF_GROUP_ask_group_data(data->group, &old_sides, &n));  
            UF_VEC3_copy(data->origin, line_data.start_point);  
            UF_VEC3_copy(corner2, line_data.end_point);  
            UF_CALL(UF_CURVE_edit_line_data(old_sides[0], &line_data));  
            UF_VEC3_copy(corner2, line_data.start_point);  
            UF_VEC3_copy(corner3, line_data.end_point);  
            UF_CALL(UF_CURVE_edit_line_data(old_sides[1], &line_data));  
            UF_VEC3_copy(corner3, line_data.start_point);  
            UF_VEC3_copy(data->origin, line_data.end_point);  
            UF_CALL(UF_CURVE_edit_line_data(old_sides[2], &line_data));  
            UF_free(old_sides);  
        }  
    }  
    /* 里海 */  
    static int filter_for_triangles(tag_t object, int type[3], void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            ii = 0,  
            n,  
            subtype,  
            otype;  
        tag_t  
            group,  
            *objects;  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group == NULL_TAG) return (UF_UI_SEL_REJECT);  
        UF_CALL(UF_GROUP_ask_group_data(group, &objects, &n));  
        if (n == 3)  
        {  
            for (ii = 0; ii < 3; ii++)  
            {  
                if (objects[ii] == object) continue;  
                UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &otype, &subtype));  
                if (otype != UF_line_type) break;  
            }  
        }  
        UF_free(objects);  
        if (ii == 3) return (UF_UI_SEL_ACCEPT);  
        else return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int select_triangles(int n_sel, tag_t *sel_objects, int n_des,  
        tag_t *des_objects, void *user_data, UF_UI_selection_p_t select)  
    {  
        int  
            descriptor;  
        tag_t  
            view;  
        TRI_data_p_t  
            data = (TRI_data_p_t)user_data;  
        UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
        if (data->group != NULL_TAG) set_highlight_group(data->group, FALSE);  
        if (descriptor & UF_UI_SEL_DESC_SINGLE_POSITION)  
        {  
            UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, data->origin));  
            data->group = NULL_TAG;  
            UF_CALL(UF_CSYS_ask_wcs(&data->csys));  
            create_edit_triangle(data);  
        }  
        else  
        {  
            UF_CALL(UF_GROUP_ask_group_of_tag(sel_objects[0], &data->group));  
            find_size_and_origin(data);  
        }  
        set_highlight_group(data->group, TRUE);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            value;  
        UF_UI_mask_t  
            mask = { UF_line_type, 0, 0 };  
        TRI_data_p_t  
            data = (TRI_data_p_t)client_data;  
        data->dialog_id = dialog_id;  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr  = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
        UF_CALL(UF_UI_set_sel_mask(value.value.selection,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));  
        UF_CALL(UF_UI_set_sel_procs(value.value.selection, filter_for_triangles,  
            select_triangles, client_data));  
        UF_CALL(UF_STYLER_free_value(&value ));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int GTAC_size_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        TRI_data_p_t  
            data = (TRI_data_p_t)client_data;  
        data->size = callback_data->value.real;  
        create_edit_triangle(data);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        TRI_data_t  
            data = { 0, NULL_TAG, NULL_TAG, 1.0, { 0,0,0 } };  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
     {GTAC_SCALE_REAL_0     , UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_size_cb},  
     {GTAC_SCALE_REAL_0     , UF_STYLER_DRAG_CB         , 0, GTAC_size_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("triangle", GTAC_cbs, GTAC_CB_COUNT, &data,  
            &resp));  
        if (data.group != NULL_TAG) set_highlight_group(data.group, FALSE);  
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

> 这段代码是一个NX Open C++应用程序，用于创建和修改等边三角形。其主要功能包括：
>
> 1. 创建等边三角形：用户可以指定三角形的中心点，程序会基于该点创建一个等边三角形。
> 2. 修改等边三角形：用户可以选择已有的三角形，程序会显示其参数，用户可以修改三角形的边长，程序会实时更新三角形。
> 3. 使用UIStyler对话框：程序使用UIStyler创建了一个对话框，其中包含一个实数输入框，用户可以通过它来指定三角形边长。程序会监听该输入框的值变化，以更新三角形。
> 4. 几何运算：程序使用了NX提供的几何运算函数，包括向量运算、坐标系变换等。
> 5. 错误处理：程序实现了错误处理机制，可以在系统日志中记录错误信息。
> 6. NX Open API封装：通过使用NX Open C++ API，实现了NX软件的二次开发。
> 7. NX Open UI：程序使用了NX Open UI组件，实现了交互式选择功能。
> 8. 代码结构：代码结构清晰，包含注释说明，方便理解和维护。
>
> 总的来说，这段代码是一个典型的NX Open C++应用程序，展示了如何利用NX Open API进行几何建模和交互式操作。
>
