### 【0004】align note with work view 对齐笔记与工作视图

#### 代码

```cpp
static void align_note_with_work_view(tag_t note)
{
    tag_t
        new_matrix_id;
    double
        wrk_view_mtx[9],
        note_origin[3];
/*  Get note origin to replace reoriented note correctly 译:将笔记的原始方向正确地替换重新定向的笔记 */
    UF_CALL(UF_DRF_ask_origin( note, note_origin ));
/*  Get work view matrix and use it as new rotation matrix for the note 译:获取工作视角矩阵，并将其用作音符的新旋转矩阵。 */
    uc6433( "", wrk_view_mtx );
    UF_CALL(UF_CSYS_create_matrix( wrk_view_mtx, &new_matrix_id ));
    UF_CALL(UF_CSYS_edit_matrix_of_object( note, new_matrix_id ));
/*  Restore note to correct origin point 译:恢复注释到正确的原点 */
    UF_CALL(UF_DRF_set_origin( note, note_origin ));
}

```

#### 代码解析

> 这段代码的主要功能是将NX中的注释对象调整为与当前工作视图对齐。具体步骤如下：
>
> 1. 首先获取注释对象当前的原始坐标。
> 2. 然后获取当前的工作视图矩阵。
> 3. 接着使用工作视图矩阵创建一个新的坐标系。
> 4. 将注释对象设置为使用新创建的坐标系，使其旋转方向与工作视图一致。
> 5. 最后将注释对象恢复到原始坐标，使其位置保持不变，只改变方向。
>
> 通过以上步骤，注释对象被调整为与当前工作视图方向一致，实现了注释与视图的对齐。这段代码在NX二次开发中经常用于实现视图注释的动态更新，以保持与视图方向的一致性。
>
