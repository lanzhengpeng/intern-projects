### 【2845】set wcs 设置工作坐标系

#### 代码

```cpp
static tag_t set_wcs(double origin[3], double dest_csys[9])
{
    tag_t
        csys,
        mx,
        orig_wcs;
    UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));
    UF_MTX3_ortho_normalize(dest_csys);
    UF_CALL(UF_CSYS_create_matrix(dest_csys, &mx));
    UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &csys));
    UF_CALL(UF_CSYS_set_wcs(csys));
    return orig_wcs;
}

```

#### 代码解析

> 这段代码是一个用于设置NX工作坐标系的函数，其主要功能如下：
>
> 1. 获取原始坐标系：使用UF_CSYS_ask_wcs函数来获取NX当前的WCS(世界坐标系)。
> 2. 创建目标坐标系：通过UF_MTX3_ortho_normalize函数对传入的目标坐标系矩阵进行正交化，然后使用UF_CSYS_create_matrix函数根据该矩阵创建一个坐标系。
> 3. 创建临时坐标系：利用UF_CSYS_create_temp_csys函数，根据给定的原点坐标和目标坐标系矩阵创建一个临时坐标系。
> 4. 设置WCS：调用UF_CSYS_set_wcs函数，将创建的临时坐标系设置为新的WCS。
> 5. 返回原始坐标系：函数返回原始WCS的tag，以便后续可以恢复WCS。
>
> 总的来说，这段代码实现了在NX中设置一个新的WCS，并保留了原始WCS以便后续恢复。通过使用UF函数进行坐标系操作，这段代码展现了NX二次开发中坐标系相关操作的典型用法。
>
