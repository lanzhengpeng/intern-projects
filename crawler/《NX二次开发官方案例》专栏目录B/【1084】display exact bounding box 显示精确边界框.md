### 【1084】display exact bounding box 显示精确边界框

#### 代码

```cpp
static void map_abs2csys(tag_t input_csys, double vec[3])
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
    FTN(uf5940)(abs_mx, csys, mx, &irc);
    UF_CALL(irc);
    FTN(uf5941)(vec, mx);
    UF_VEC3_sub(vec, csys, vec);
}
static void display_temporary_box(double llc[3], double urc[3], tag_t csys)
{
    double
        dirs[9] = { 1,0,0, 0,1,0, 0,0,1 },
        delta_x[3],
        delta_y[3],
        delta_z[3],
        end[3],
        mag,
        sizes[3],
        start[3];
    tag_t
        csys_mx;
    UF_OBJ_disp_props_t
        props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,
            UF_OBJ_FONT_SOLID, FALSE};
    UF_VEC3_copy(llc, start);
    UF_VEC3_copy(urc, end);
    if (csys != NULL_TAG)
    {
        UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));
        UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);
        UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);
        UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);
        map_abs2csys(csys, start);
        map_abs2csys(csys, end);
    }
    UF_VEC3_sub(end, start, sizes);
    ECHO("Box is %.3f X %.3f X %.3f\n", sizes[0], sizes[1], sizes[2]);
    UF_VEC3_scale(sizes[0], &dirs[0], delta_x);
    UF_VEC3_scale(sizes[1], &dirs[3], delta_y);
    UF_VEC3_scale(sizes[2], &dirs[6], delta_z);
    UF_VEC3_add(llc, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_add(llc, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_add(llc, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_sub(urc, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_sub(urc, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_sub(urc, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_add(llc, delta_x, start);
    UF_VEC3_add(start, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_sub(end, delta_x, start);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_sub(end, delta_z, start);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_UUF_MODL_ask_bounding_box_exactSE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
}
static void display_exact_bounding_box(tag_t object, tag_t csys)
{
    int
        ii,jj;
    double
        abs[9] = { 1,0,0, 0,1,0, 0,0,1 },
        min[3],
        dir[3][3],
        dist[3],
        max[3],
        zero[3] = { 0,0,0};
    tag_t
        mx;
    if (csys == NULL_TAG)
    {
        UF_CALL(UF_CSYS_create_matrix(abs, &mx));
        UF_CALL(UF_CSYS_create_temp_csys(zero, mx, &csys));
    }
    UF_CALL(UF_MODL_ask_bounding_box_exact(object, csys, min, dir, dist ));
    for (ii = 0; ii < 3; ii++)
    {
        max[ii] = min[ii];
        for (jj = 0; jj < 3; jj++)
            max[ii] += dir[jj][ii] * dist[jj];
    }
    display_temporary_box(min, max, csys);
}

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要用于显示NX对象在指定坐标系下的精确边界框。主要功能包括：
>
> 1. 坐标系转换：map_abs2csys函数用于将绝对坐标系下的点转换到指定坐标系下。
> 2. 显示边界框：display_temporary_box函数用于在指定坐标系下显示一个边界框，包括8个顶点和12条边。
> 3. 获取边界框信息：display_exact_bounding_box函数用于获取NX对象的精确边界框信息，包括最小点、方向和距离，然后调用display_temporary_box函数进行显示。
> 4. 坐标系创建：如果未指定坐标系，代码会创建一个绝对坐标系。
> 5. NX API调用：代码中使用了NX提供的API函数，如UF_MODL_ask_bounding_box_exact用于获取精确边界框信息，UF_DISP_display_temporary_line用于显示临时线。
> 6. NX对象操作：通过二次开发代码可以获取NX对象信息，并进行显示等操作。
>
> 总的来说，这段代码通过二次开发实现了NX对象精确边界框的显示，为用户提供了一个便捷的功能。
>
