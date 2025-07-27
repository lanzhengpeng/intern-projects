### 【2981】transform csys to csys 将坐标系from_csys转换到坐标系to_csys

#### 代码

```cpp
static void transform_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[16])
{
    double
        csys[9],
        orig[3],
        tx1[16],
        tx2[16],
        tx3[16],
        tx4[16],
        v[3];
    int
        ii;
    tag_t
        csys_mx;
    UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    tx1[0] = csys[0];
    tx1[1] = csys[1];
    tx1[2] = csys[2];
    tx1[3] = 0;
    tx1[4] = csys[3];
    tx1[5] = csys[4];
    tx1[6] = csys[5];
    tx1[7] = 0;
    tx1[8] = csys[6];
    tx1[9] = csys[7];
    tx1[10] = csys[8];
    tx1[11] = 0;
    /* set up to translate from reference csys back to absolute 译:根据参考坐标系转换回绝对坐标系的设置，进行翻译。 */
    for (ii=0; ii<3; ii++) v[ii] = -orig[ii];
    FTN(uf5943)(v, tx2);
    /* combine this with the rotation matrix from the reference csys 译:将这个与参考坐标系的旋转矩阵结合起来 */
    FTN(uf5942)(tx2, tx1, tx3);
    UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    /* Invert the rotation from the destination matrix 译:反转目的地矩阵的旋转。 */
    tx2[0] = csys[0];
    tx2[1] = csys[3];
    tx2[2] = csys[6];
    tx2[3] = 0;
    tx2[4] = csys[1];
    tx2[5] = csys[4];
    tx2[6] = csys[7];
    tx2[7] = 0;
    tx2[8] = csys[2];
    tx2[9] = csys[5];
    tx2[10] = csys[8];
    tx2[11] = 0;
    /* set up to translate from abs to the destination csys 译:根据abs到目标csys的设置，进行翻译。 */
    FTN(uf5943)(orig, tx1);
    /* combine this with the inverted rotation csys above 译:翻译：将这个与上面反转的旋转坐标系结合。 */
    FTN(uf5942)(tx2, tx1, tx4);
    /* and the one from the reference csys 译:参考坐标系中的那个 */
    FTN(uf5942)(tx3, tx4, mx);
}
static void transform_csys_to_csys(tag_t from_csys, tag_t to_csys, int n,
    tag_t *objects)
{
    int
        move = 1,
        work = -1,
        off = 2,
        resp;
    double
        mx[16];
    transform_csys2csys(from_csys, to_csys, mx);
    FTN(uf5947)(mx, objects, &n, &move, &work, &off, NULL, NULL, &resp);
}

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于将NX中的对象从一个坐标系转换到另一个坐标系。
>
> 代码主要包含以下部分：
>
> 1. transform_csys2csys函数：该函数负责计算从一个坐标系到另一个坐标系的变换矩阵。首先获取参考坐标系和目标坐标系的旋转矩阵和原点，然后通过矩阵运算计算出变换矩阵。
> 2. transform_csys_to_csys函数：该函数负责应用变换矩阵，将对象从一个坐标系转换到另一个坐标系。它调用NX的UF_MODL_transform函数，将计算出的变换矩阵应用到指定的对象数组上。
> 3. 主要思路：通过获取参考坐标系和目标坐标系的旋转矩阵和原点信息，计算出两者之间的变换矩阵。然后利用这个变换矩阵，通过UF_MODL_transform函数将对象从一个坐标系转换到另一个坐标系。
> 4. 注意事项：代码中使用了NX的内部函数FTN()，这可能涉及到一些内部实现。同时，代码中省略了错误处理逻辑，实际使用时需要添加。
>
> 总体来说，这段代码通过矩阵运算实现了NX中对象在坐标系之间的转换，为NX的二次开发提供了一定的参考价值。
>
