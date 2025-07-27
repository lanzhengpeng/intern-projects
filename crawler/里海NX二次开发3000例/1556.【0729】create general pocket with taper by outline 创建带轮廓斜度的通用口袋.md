### 【0729】create general pocket with taper by outline 创建带轮廓斜度的通用口袋

#### 代码

```cpp
    /*HEAD CREATE_GENERAL_POCKET_WITH_TAPER_BY_OUTLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意：UF_print_syslog 是在 V18 中新增的。 */  
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
    static void do_it(void)  
    {  
        int    units= UF_PART_METRIC;  
        int    num_faces,ii;  
        char   *part_name="pocket";  
        char   radius[133] = {"0.0"};  
        char   *edge_lens[] = {"100.0","50.0","100.0"};  
        double origin[3]={0.0,0.0,0.0};  
        double face_param[2],face_point[3],face_u_deriv1[3];  
        double face_v_deriv1[3],face_u_deriv2[3],face_v_deriv2[3];  
        double face_normal[3],face_radii[2];  
        double matrix_values[9] = { 1, 0, 0, 0, 0, 1, 0, 1, 0 };  
        uf_list_p_t face_list;  
        UF_CURVE_line_t line1, line2, line3;  
        UF_CURVE_arc_t arc1, arc2, arc3;  
        UF_MODL_faces_t placement_faces,selected_floor_faces;  
        UF_MODL_offset_trans_faces_t floor_faces;  
        UF_MODL_pocketpad_outline_t placement_outline;  
        UF_MODL_project_curves_t outline_curves;  
        UF_MODL_parm_t placement_radius,floor_radius;  
        UF_MODL_vector_t placement_proj_vector =  
                    {0,UF_MODL_VECTOR_DIRECTION,  
                     {0,1,0}};  
        UF_MODL_parm_t taper_angle[6];  
        UF_MODL_law_spine_t law_spine[6];  
        tag_t  part_tag,block_tag;  
        tag_t  place_face[1],floor_face[1],general_pocket_tag;  
        tag_t  placement_curves[6],face_tag;  
        tag_t matrix_tag;  
        UF_PART_new(part_name, units, &part_tag);  
    /* create the block 译:创建区块 */  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN,  
                              origin,  
                              edge_lens,  
                              &block_tag));  
    /* create the matrix for arcs 译:构建弧形矩阵 */  
        UF_CALL(UF_CSYS_create_matrix(matrix_values, &matrix_tag));  
    /* create the placement outline curves 译:创建定位轮廓曲线 */  
    /* create an arc 译:创建一个弧 */  
        arc1.matrix_tag = matrix_tag;  
        arc1.start_angle = 0.0;  
        arc1.end_angle = 90.0 * DEGRA;  
        arc1.arc_center[0] = 75.0;  
        arc1.arc_center[1] = 70.0;  
        arc1.arc_center[2] = 0.0;  
        arc1.radius =  10.0;  
        UF_CALL(UF_CURVE_create_arc(&arc1, &placement_curves[0]));  
    /* create a straight line 译:划直线 */  
        line1.start_point[0] =  85.0;  
        line1.start_point[1] =  0.0;  
        line1.start_point[2] =  70.0;  
        line1.end_point[0] =  85.0;  
        line1.end_point[1] =  0.0;  
        line1.end_point[2] =  50.0;  
        UF_CALL(UF_CURVE_create_line(&line1, &(placement_curves[1])));  
    /* create an arc 译:创建一个弧 */  
        arc2.matrix_tag = matrix_tag;  
        arc2.start_angle =  248.198590513650 * DEGRA;  
        arc2.end_angle = 360.0 * DEGRA;  
        arc2.arc_center[0] = 75.0;  
        arc2.arc_center[1] = 50.0;  
        arc2.arc_center[2] = 0.0;  
        arc2.radius =  10.0;  
        UF_CALL(UF_CURVE_create_arc(&arc2, &placement_curves[2]));  
    /* create a straight line 译:创建一条直线 */  
        line2.start_point[0] =  71.2860932364590;  
        line2.start_point[1] =  0.0;  
        line2.start_point[2] =  40.7152330911470;  
        line2.end_point[0] =  21.2860932364590;  
        line2.end_point[1] =  0.0;  
        line2.end_point[2] =  60.7152330911470;  
        UF_CALL(UF_CURVE_create_line(&line2, &(placement_curves[3])));  
    /* create an arc 译:创建一个弧 */  
        arc3.matrix_tag = matrix_tag;  
        arc3.start_angle = 90.0 * DEGRA;  
        arc3.end_angle =  248.198590513650 * DEGRA;  
        arc3.arc_center[0] = 25.0;  
        arc3.arc_center[1] = 70.0;  
        arc3.arc_center[2] = 0.0;  
        arc3.radius =  10.0;  
        UF_CALL(UF_CURVE_create_arc(&arc3, &placement_curves[4]));  
    /* create a straight line 译:create a straight line 翻译为：创建一条直线。 */  
        line3.start_point[0] =  25.0;  
        line3.start_point[1] =  0.0;  
        line3.start_point[2] =  80.0;  
        line3.end_point[0] =  75.0;  
        line3.end_point[1] =  0.0;  
        line3.end_point[2] =  80.0;  
        UF_CALL(UF_CURVE_create_line(&line3, &(placement_curves[5])));  
    /* determine the placement face from the block,it is the    译:根据块确定放置面，它是... */  
    /* face whose normal is mostly in the positive y_direction, 译:正面y方向的法线大部分的面部 */  
    /* the floor face is the face whose normal is mostly   译:地面面是法线大部分指向地面的面。 */  
    /* in the negative y_direction                              译:in the negative y_direction的翻译是：负的y方向 */  
        UF_MODL_ask_feat_faces(block_tag,  
                               &face_list);  
        UF_MODL_ask_list_count(face_list,  
                               &num_faces);  
        face_param[0] = 0.5;  
        face_param[1] = 0.5;  
        for (ii = 0;ii < num_faces;ii++)  
        {  
          UF_MODL_ask_list_item(face_list,  
                                ii,  
                                &face_tag);  
          UF_MODL_ask_face_props(face_tag,  
                                 face_param,  
                                 face_point,  
                                 face_u_deriv1,face_v_deriv1,  
                                 face_u_deriv2,face_v_deriv2,  
                                 face_normal,face_radii);  
          if (face_normal[1] > 0.75)  
            place_face[0] = face_tag;  
          if (face_normal[1] < -0.75)  
            floor_face[0] = face_tag;  
        }  
        UF_MODL_delete_list(&face_list);  
    /* set up input data to create the pocket 译:建立输入数据来创建口袋 */  
    /* set up data for placement faces 译:设置放置面的数据 */  
        placement_faces.number_of_faces = 1;  
        placement_faces.faces = place_face;  
        placement_faces.face_extension =  
                            UF_MODL_FACE_EXTENSION_NONE;  
    /* set up data for floor faces 译:为地面面设置数据 */  
        selected_floor_faces.number_of_faces = 1;  
        selected_floor_faces.faces = floor_face;  
        selected_floor_faces.face_extension =  
                            UF_MODL_FACE_EXTENSION_NONE;  
        floor_faces.faces = &selected_floor_faces;  
        floor_faces.method_type = UF_MODL_OFFSET_OF_FACES;  
        strcpy(floor_faces.distance,"0.0");  
        floor_faces.direction = NULL;  
    /* set up data for placement outline 译:设置数据放置轮廓 */  
        outline_curves.number_of_curves = 6;  
        outline_curves.curves = placement_curves;  
        outline_curves.curve_projection_type =  
                    UF_MODL_PROJ_ALONG_VECTOR;  
        outline_curves.curve_projection_vector =  
                            &placement_proj_vector;  
        placement_outline.outline_type = UF_MODL_SINGLE_OUTLINE;  
        placement_outline.defined_by.single_outline.  
            outline_curves = &outline_curves;  
        placement_outline.defined_by.single_outline.  
            curves_represent = UF_MODL_TANGENT_AT_END_OF_BLEND;  
        placement_outline.defined_by.single_outline.  
            outline_represents = UF_MODL_SHAPE_AT_PLACEMENT;  
        placement_outline.defined_by.single_outline.  
            taper_angle = taper_angle;  
        taper_angle[0].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[0].defined_by.law_spine.law = &law_spine[0];  
        UF_MODL_init_string_list(&law_spine[0].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[0].spine_profile);  
        law_spine[0].spine_profile.string[0] = 1;  
        law_spine[0].spine_profile.dir[0] = 1;  
        law_spine[0].spine_profile.id[0] = placement_curves[0];  
        law_spine[0].method_type = UF_MODL_LAW_LINEAR;  
        strcpy(law_spine[0].defined_by.linear_cubic.start_value, "5.0");  
        strcpy(law_spine[0].defined_by.linear_cubic.end_value, "6.0");  
        taper_angle[1].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[1].defined_by.law_spine.law = &law_spine[1];  
        UF_MODL_init_string_list(&law_spine[1].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[1].spine_profile);  
        law_spine[1].spine_profile.string[0] = 1;  
        law_spine[1].spine_profile.dir[0] = 1;  
        law_spine[1].spine_profile.id[0] = placement_curves[1];  
        law_spine[1].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[1].defined_by.constant.value, "5.0");  
        taper_angle[2].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[2].defined_by.law_spine.law = &law_spine[2];  
        UF_MODL_init_string_list(&law_spine[2].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[2].spine_profile);  
        law_spine[2].spine_profile.string[0] = 1;  
        law_spine[2].spine_profile.dir[0] = 1;  
        law_spine[2].spine_profile.id[0] = placement_curves[2];  
        law_spine[2].method_type = UF_MODL_LAW_LINEAR;  
        strcpy(law_spine[2].defined_by.linear_cubic.start_value, "7.0");  
        strcpy(law_spine[2].defined_by.linear_cubic.end_value, "5.0");  
        taper_angle[3].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[3].defined_by.law_spine.law = &law_spine[3];  
        UF_MODL_init_string_list(&law_spine[3].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[3].spine_profile);  
        law_spine[3].spine_profile.string[0] = 1;  
        law_spine[3].spine_profile.dir[0] = 1;  
        law_spine[3].spine_profile.id[0] = placement_curves[3];  
        law_spine[3].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[3].defined_by.constant.value, "7.0");  
        taper_angle[4].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[4].defined_by.law_spine.law = &law_spine[4];  
        UF_MODL_init_string_list(&law_spine[4].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[4].spine_profile);  
        law_spine[4].spine_profile.string[0] = 1;  
        law_spine[4].spine_profile.dir[0] = 1;  
        law_spine[4].spine_profile.id[0] = placement_curves[4];  
        law_spine[4].method_type = UF_MODL_LAW_LINEAR;  
        strcpy(law_spine[4].defined_by.linear_cubic.start_value, "6.0");  
        strcpy(law_spine[4].defined_by.linear_cubic.end_value, "7.0");  
        taper_angle[5].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[5].defined_by.law_spine.law = &law_spine[5];  
        UF_MODL_init_string_list(&law_spine[5].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[5].spine_profile);  
        law_spine[5].spine_profile.string[0] = 1;  
        law_spine[5].spine_profile.dir[0] = 1;  
        law_spine[5].spine_profile.id[0] = placement_curves[5];  
        law_spine[5].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[5].defined_by.constant.value, "6.0");  
        placement_outline.defined_by.single_outline.  
            taper_relative_to = UF_MODL_TAPER_BY_OUTLINE_FACE_NORMALS;  
        placement_outline.defined_by.single_outline.  
            taper_vector = NULL;  
    /* set up placement and floor radius 译:设置放置和楼层半径 */  
        placement_radius.method_type = UF_MODL_PARM_CONSTANT;  
        strcpy(placement_radius.defined_by.constant.value,radius);  
        floor_radius.method_type = UF_MODL_PARM_CONSTANT;  
        strcpy(floor_radius.defined_by.constant.value,radius);  
    /* call the function to create the pocket 译:调用函数创建口袋 */  
        UF_CALL(UF_MODL_create_general_pocket(&placement_faces,  
                                      &floor_faces,  
                                      &placement_outline,  
                                      &placement_radius,  
                                      &floor_radius,  
                                      &general_pocket_tag));  
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

> 这段代码是用于创建具有锥度的通用口袋的NX二次开发代码。其主要功能包括：
>
> 1. 创建一个块，用于定义口袋的放置面和底面。
> 2. 定义一个坐标系，用于创建放置轮廓曲线。
> 3. 创建放置轮廓曲线，包括弧线和直线，这些曲线构成了口袋的轮廓。
> 4. 确定放置面和底面。放置面是法线指向正y方向的块面，底面是法线指向负y方向的块面。
> 5. 设置放置轮廓数据，包括曲线、曲线投影方向、轮廓表示的形状、锥度角度等信息。
> 6. 设置放置半径和底面半径。
> 7. 调用UF_MODL_create_general_pocket函数创建具有锥度的通用口袋。
> 8. 输出错误信息，便于调试。
> 9. 提供UF_terminate函数终止NX。
> 10. 提供ufusr_ask_unload函数卸载用户自定义函数。
>
> 这段代码实现了在NX中创建具有锥度的通用口袋的功能，提供了完整的流程和详细的错误处理。
>
