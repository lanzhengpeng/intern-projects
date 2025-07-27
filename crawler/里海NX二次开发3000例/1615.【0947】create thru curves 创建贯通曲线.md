### 【0947】create thru curves 创建贯通曲线

#### 代码

```cpp
static void create_thru_curves(void)
{
    UF_CURVE_line_t
        line_coords;
    UF_STRING_t
        s_section,
        s_spine;
    int
        alignment = 1,
        body_type = UF_MODL_SOLID_BODY,
        c_flag[2] = { 0,0 },
        patch = 1,
        vdegree = 3,
        vstatus = 0;
    double
        tol[3] = { 0, 0, 0 },
        value[6] = { 0,0,0, 0,0,0 };
    tag_t
        body_id,
        c_face[2] = { NULL_TAG, NULL_TAG };
    /*  Create the section curves 译:创建截面曲线 */
    UF_MODL_init_string_list(&s_section);
    UF_MODL_create_string_list(3, 12, &s_section);
    s_section.string[0] = 4;
    s_section.dir[0] = 1;
    UF_VEC3_copy(tol, line_coords.start_point);
    UF_VEC3_copy(tol, line_coords.end_point);
    line_coords.end_point[1] = 1;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[0] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 1;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[1] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[1] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[2] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[3] ));
    s_section.string[1] = 4;
    s_section.dir[1] = 1;
    UF_VEC3_copy(tol, line_coords.start_point);
    UF_VEC3_copy(tol, line_coords.end_point);
    line_coords.start_point[0] = 10;
    line_coords.end_point[0] = 10;
    line_coords.end_point[1] = 2;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[4] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 2;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[5] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[1] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[6] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[7] ));
    s_section.string[2] = 4;
    s_section.dir[2] = 1;
    UF_VEC3_copy(tol, line_coords.start_point);
    UF_VEC3_copy(tol, line_coords.end_point);
    line_coords.start_point[0] = 20;
    line_coords.end_point[0] = 20;
    line_coords.end_point[1] = 3;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[8] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 3;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[9] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[1] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[10] ));
    UF_VEC3_copy(line_coords.end_point, line_coords.start_point);
    line_coords.end_point[2] = 0;
    UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[11] ));
    /*  No spine 译:没有脊梁骨。 */
    UF_MODL_init_string_list(&s_spine);
    UF_CALL( UF_MODL_create_thru_curves ( &s_section, &s_spine, &patch,
        &alignment, value, &vdegree, &vstatus, &body_type, UF_NULLSIGN,
        tol, c_face, c_flag, &body_id ));
    UF_MODL_free_string_list(&s_section);
    UF_MODL_free_string_list(&s_spine);
}

```

#### 代码解析

> 这是段用于创建NX中的通过曲线的二次开发代码，主要功能如下：
>
> 1. 首先，定义了线段的起点和终点坐标，并创建了12条线段，这些线段构成了截面曲线。
> 2. 通过UF_MODL_create_thru_curves函数，使用这些截面曲线生成了一个实体模型。参数包括：s_section: 存储截面曲线的数组s_spine: 存储脊线的数组，此处为NULL，表示没有脊线patch: 生成实体的方法，此处为1，表示使用通过曲线alignment: 曲线对齐方式，此处为1，表示按曲线方向对齐value: 控制曲面参数，此处为0vdegree: 曲面次数，此处为3vstatus: 曲面状态，此处为0body_type: 生成实体的类型，此处为实体tol: 公差，此处为0c_face: 生成实体的约束面，此处为NULLc_flag: 约束面标志，此处为0body_id: 生成实体的id
> 3. s_section: 存储截面曲线的数组
> 4. s_spine: 存储脊线的数组，此处为NULL，表示没有脊线
> 5. patch: 生成实体的方法，此处为1，表示使用通过曲线
> 6. alignment: 曲线对齐方式，此处为1，表示按曲线方向对齐
> 7. value: 控制曲面参数，此处为0
> 8. vdegree: 曲面次数，此处为3
> 9. vstatus: 曲面状态，此处为0
> 10. body_type: 生成实体的类型，此处为实体
> 11. tol: 公差，此处为0
> 12. c_face: 生成实体的约束面，此处为NULL
> 13. c_flag: 约束面标志，此处为0
> 14. body_id: 生成实体的id
> 15. 最后，释放了存储截面曲线和脊线的内存。
>
> 总的来说，这段代码通过创建截面曲线，生成了一个实体模型。
>
