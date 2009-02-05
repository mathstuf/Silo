#include <stdio.h>
#include <silo.h>
#include <sys/time.h>
#include <sys/timeb.h>

/* To compile this program on hyper, here is the command:
 *
 *   cc -o ts -I/usr/local/silo/4.2/irix64/n32/include 
 *      -L/usr/local/silo/4.2/irix64/n32/lib -DNEW_SILO TestSilo.c -lsilo -lm
 * 
 * Note that the -DNEW_SILO should be removed if you want to see what would
 * happen without using the read masks.
 *
 * Programmer: Brad Whitlock
 * Date:       Thu May 17 13:00:50 PST 2001
 *
 */
#ifdef NEW_SILO
void    printMask(FILE * fp, long mask);
#endif
int     test_readmat(DBfile * dbfile, const char *testName, long mask);
int     test_readpointmesh(DBfile * dbfile, long mask);
int     test_readpointvar(DBfile * dbfile, long mask);
int     test_readquadmesh(DBfile * dbfile, long mask);
int     test_readquadvar(DBfile * dbfile, long mask);
int     test_readucdmesh(DBfile * dbfile, const char *testName, long mask);
int     test_readucdvar(DBfile * dbfile, long mask);
int     test_readfacelist(DBfile * dbfile, long mask);
int     test_readzonelist(DBfile * dbfile, long mask);

void    ResetTime(void);
int     ElapsedTime(void);
void    printMaterial(const char *routine, DBmaterial * mat);
void    printTimes(int *ms);

/* Variables to hold time data. */
struct timeb start_time;
struct timeb end_time;

int
main(int argc, char *argv[])
{
    DBfile *dbfile = NULL;
    int     ms[33];
    char   *files[] = { "./rect2d.pdb",
        "./point2d.pdb",
        "./globe.pdb"
    };
    int maskindex = 0;

#ifdef NEW_SILO
    /* Set the masks used for the tests. */
    long    mask[] = {
        DBAll,
        DBMatMatnos,
        DBMatMatlist,
        DBMatMixList,
        DBNone,
        /* ----- */
        DBAll,
        DBQMCoords,
        DBNone,
        /* ----- */
        DBAll,
        DBQVData,
        DBNone,
        /* ----- */
        DBAll,
        DBPMCoords,
        DBNone,
        /* ----- */
        DBAll,
        DBPVData,
        DBNone,
        /* ----- */
        DBAll,
        DBUMCoords,
        DBUMFacelist,
        DBUMZonelist,
        DBNone,
        /* ----- */
        DBAll,
        DBUVData,
        DBNone,
        /* ----- */
        DBAll,
        DBFacelistInfo,
        DBNone,
        /* ----- */
        DBAll,
        DBZonelistInfo,
        DBNone,
    };

    printf("NOTE: The times listed here do not take any caching into\n");
    printf("account.  Thus, the first time listed in each section may be\n");
    printf("extraordinarily large.  For an accurate timing test, this cache\n");
    printf("would have to be disabled.\n\n");

    printMask(stdout, DBGetDataReadMask());
    printf("\n\n");
#else
    long    mask[24];

    {
        int     i;

        for (i = 0; i < 23; ++i)
            mask[i] = 0L;
    }
#endif

    /**************************************************************************
     *
     * Test the material read flags.
     *
     *************************************************************************/
    dbfile = DBOpen(files[0], DB_UNKNOWN, DB_READ);
    if (dbfile == NULL)
    {
        fprintf(stderr, "The file %s could not be opened!\n", files[0]);
        return 1;
    }

    /* Try reading material. */
    ms[maskindex] = test_readmat(dbfile, "test_readmatnos", mask[maskindex]);
    printf("Reading material for %s took %d ms.\n\n", files[0],
           ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readmat(dbfile, "test_readmatnos", mask[maskindex]);
    printf("Reading material numbers for %s took %d ms.\n\n", files[0],
           ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readmat(dbfile, "test_readmatlist", mask[maskindex]);
    printf("Reading material list for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readmat(dbfile, "test_readmatmixlist", mask[maskindex]);
    printf("Reading material mix list for %s took %d ms.\n\n", files[0],
           ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readmat(dbfile, "test_readmatlist", mask[maskindex]);
    printf("Reading material list for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");


    /**************************************************************************
     *
     * Test the quad mesh read flags.
     *
     *************************************************************************/
    /* Try reading the quad mesh. */
    ms[maskindex] = test_readquadmesh(dbfile, mask[maskindex]);
    printf("Reading quad mesh for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readquadmesh(dbfile, mask[maskindex]);
    printf("Reading quad mesh for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readquadmesh(dbfile, mask[maskindex]);
    printf("Reading quad mesh for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    /* Try reading a quad var. */
    ms[maskindex] = test_readquadvar(dbfile, mask[maskindex]);
    printf("Reading quad var for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readquadvar(dbfile, mask[maskindex]);
    printf("Reading quad var for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readquadvar(dbfile, mask[maskindex]);
    printf("Reading quad var for %s took %d ms.\n\n", files[0], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    DBClose(dbfile);

    /**************************************************************************
     *
     * Test the point mesh read flags.
     *
     *************************************************************************/
    dbfile = DBOpen(files[1], DB_UNKNOWN, DB_READ);
    if (dbfile == NULL)
    {
        fprintf(stderr, "The file %s could not be opened!\n", files[1]);
        return 1;
    }

    /* Try reading the point mesh. */
    ms[maskindex] = test_readpointmesh(dbfile, mask[maskindex]);
    printf("Reading point mesh for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readpointmesh(dbfile, mask[maskindex]);
    printf("Reading point mesh for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readpointmesh(dbfile, mask[maskindex]);
    printf("Reading point mesh for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    /* Try reading a point var. */
    ms[maskindex] = test_readpointvar(dbfile, mask[maskindex]);
    printf("Reading point var for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readpointvar(dbfile, mask[maskindex]);
    printf("Reading point var for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readpointvar(dbfile, mask[maskindex]);
    printf("Reading point var for %s took %d ms.\n\n", files[1], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");
    DBClose(dbfile);

    /**************************************************************************
     *
     * Test the ucd mesh read flags.
     *
     *************************************************************************/
    dbfile = DBOpen(files[2], DB_UNKNOWN, DB_READ);
    if (dbfile == NULL)
    {
        fprintf(stderr, "The file %s could not be opened!\n", files[2]);
        return 1;
    }

    /* Try reading the ucd mesh. */
    ms[maskindex] = test_readucdmesh(dbfile, "test_readucdmeshnone", mask[maskindex]);
    printf("Reading ucd mesh for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdmesh(dbfile, "test_readucdmeshcoords", mask[maskindex]);
    printf("Reading ucd mesh for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdmesh(dbfile, "test_readucdmeshfaces", mask[maskindex]);
    printf("Reading ucd mesh for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdmesh(dbfile, "test_readucdmeshzones", mask[maskindex]);
    printf("Reading ucd mesh for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdmesh(dbfile, "test_readucdmeshzones", mask[maskindex]);
    printf("Reading ucd mesh for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    /* Try reading a ucd var. */
    ms[maskindex] = test_readucdvar(dbfile, mask[maskindex]);
    printf("Reading ucd var for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdvar(dbfile, mask[maskindex]);
    printf("Reading ucd var for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readucdvar(dbfile, mask[maskindex]);
    printf("Reading ucd var for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    /* Try reading a facelist. */
    ms[maskindex] = test_readfacelist(dbfile, mask[maskindex]);
    printf("Reading face list for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readfacelist(dbfile, mask[maskindex]);
    printf("Reading face list for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readfacelist(dbfile, mask[maskindex]);
    printf("Reading face list for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    printf("-----------------------------\n");

    /* Try reading a zonelist. */
    ms[maskindex] = test_readzonelist(dbfile, mask[maskindex]);
    printf("Reading zone list for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    ms[maskindex] = test_readzonelist(dbfile, mask[maskindex]);
    printf("Reading zone list for %s took %d ms.\n\n", files[2], ms[maskindex]);
    maskindex++;
    DBClose(dbfile);

    /* Print the times. */
    printTimes(ms);

    return 0;
}

int
test_readmat(DBfile * dbfile, const char *testName, long mask)
{
    int     ms;
    DBmaterial *mat = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("%s: ", testName);
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the material list and print the material struct. */
    mat = DBGetMaterial(dbfile, "mat1");
    ms = ElapsedTime();
    printMaterial(testName, mat);
    DBFreeMaterial(mat);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

void
printMaterial(const char *routine, DBmaterial * mat)
{
    if (mat != NULL)
    {
        int     i;

        printf("%s: mat {\n", routine);
        printf("    nmat=%d\n", mat->nmat);
        if (mat->matnos != NULL)
        {
            printf("    matnos = 0x%p = {", mat->matnos);
            for (i = 0; i < mat->nmat; ++i)
                printf("%d, ", mat->matnos[i]);
            printf("}\n");
        } else
        {
            printf("    matnos = 0x%p\n", mat->matnos);
        }
        printf("    matlist = 0x%p\n", mat->matlist);
        printf("    mix_vf = 0x%p\n", mat->mix_vf);
        printf("    mix_next = 0x%p\n", mat->mix_next);
        printf("    mix_mat = 0x%p\n", mat->mix_mat);
        printf("    mix_zone = 0x%p\n", mat->mix_zone);
        printf("};\n");
    } else
    {
        fprintf(stderr, "%s: mat = NULL!\n", routine);
    }
}

int
test_readpointmesh(DBfile * dbfile, long mask)
{
    int     ms;
    DBpointmesh *pmesh = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readpointmesh: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the material list and print the material struct. */
    pmesh = DBGetPointmesh(dbfile, "dir1/pmesh");
    ms = ElapsedTime();
    if (pmesh != NULL)
    {
        printf("test_readpointmesh: pmesh = {\n");
        printf("  ...\n");
        printf("    name = %s\n", pmesh->name);
        printf("  ...\n");
        printf("    coords[0] = 0x%p\n", pmesh->coords[0]);
        printf("    coords[1] = 0x%p\n", pmesh->coords[1]);
        printf("    coords[2] = 0x%p\n", pmesh->coords[2]);
        printf("  ...\n");
        printf("    nels = %d\n", pmesh->nels);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readpointmesh: pmesh = NULL!\n");
    }
    DBFreePointmesh(pmesh);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readpointvar(DBfile * dbfile, long mask)
{
    int     ms;
    DBmeshvar *mvar = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readpointvar: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the material list and print the material struct. */
    mvar = DBGetPointvar(dbfile, "dir1/d");
    ms = ElapsedTime();
    if (mvar != NULL)
    {
        printf("test_readpointvar: mvar = {\n");
        printf("  ...\n");
        printf("    vals = 0x%p\n", mvar->vals);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readpointvar: mvar = NULL!\n");
    }

#ifdef NEW_SILO
    /* This check gets us around a crash! */
    if (mask != DBNone)
#endif
        DBFreeMeshvar(mvar);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readquadmesh(DBfile * dbfile, long mask)
{
    int     ms;
    DBquadmesh *qmesh = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readquadmesh: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the quad mesh and print the quadmesh struct. */
    qmesh = DBGetQuadmesh(dbfile, "quadmesh2d");
    ms = ElapsedTime();
    if (qmesh != NULL)
    {
        printf("test_readquadmesh: qmesh = {\n");
        printf("  ...\n");
        printf("    name = %s\n", qmesh->name);
        printf("  ...\n");
        printf("    coords[0] = 0x%p\n", qmesh->coords[0]);
        printf("    coords[1] = 0x%p\n", qmesh->coords[1]);
        printf("    coords[2] = 0x%p\n", qmesh->coords[2]);
        printf("  ...\n");
        printf("    nnodes = %d\n", qmesh->nnodes);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readquadmesh: qmesh = NULL!\n");
    }
    DBFreeQuadmesh(qmesh);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readquadvar(DBfile * dbfile, long mask)
{
    int     ms;
    DBquadvar *qvar = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readquadvar: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the quad var and print the struct. */
    qvar = DBGetQuadvar(dbfile, "d");
    ms = ElapsedTime();
    if (qvar != NULL)
    {
        printf("test_readquadvar: qvar = {\n");
        printf("  ...\n");
        printf("    vals = 0x%p\n", qvar->vals);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readquadvar: qvar = NULL!\n");
    }

#ifdef NEW_SILO
    /* This check gets us around a crash! */
    if (mask != DBNone)
#endif
        DBFreeQuadvar(qvar);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readucdmesh(DBfile * dbfile, const char *testName, long mask)
{
    int     ms;
    DBucdmesh *umesh = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("%s: ", testName);
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the ucd mesh and print out the struct. */
    umesh = DBGetUcdmesh(dbfile, "mesh1");
    ms = ElapsedTime();
    if (umesh != NULL)
    {
        printf("%s: umesh = {\n", testName);
        printf("  ...\n");
        printf("    name = %s\n", umesh->name);
        printf("  ...\n");
        printf("    coords[0] = 0x%p\n", umesh->coords[0]);
        printf("    coords[1] = 0x%p\n", umesh->coords[1]);
        printf("    coords[2] = 0x%p\n", umesh->coords[2]);
        printf("  ...\n");
        printf("    faces = 0x%p\n", umesh->faces);
        printf("    zones = 0x%p\n", umesh->zones);
        if (umesh->zones != NULL)
            printf("    zones->nodelist = 0x%p\n", umesh->zones->nodelist);
        printf("    edges = 0x%p\n", umesh->edges);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "%s: umesh = NULL!\n", testName);
    }
    DBFreeUcdmesh(umesh);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readucdvar(DBfile * dbfile, long mask)
{
    int     ms;
    DBucdvar *uvar = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readucdvar: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the quad var and print the struct. */
    uvar = DBGetUcdvar(dbfile, "u");
    ms = ElapsedTime();
    if (uvar != NULL)
    {
        printf("test_readucdvar: uvar = {\n");
        printf("  ...\n");
        printf("    vals = 0x%p\n", uvar->vals);
        printf("  ...\n");
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readucdvar: uvar = NULL!\n");
    }

#ifdef NEW_SILO
    /* This check gets us around a crash! */
    if (mask != DBNone)
#endif
        DBFreeUcdvar(uvar);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readfacelist(DBfile * dbfile, long mask)
{
    int     ms;
    DBfacelist *fl = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readfacelist: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the face list and print the struct. */
    fl = DBGetFacelist(dbfile, "fl");
    ms = ElapsedTime();
    if (fl != NULL)
    {
        printf("test_readfacelist: fl = {\n");
        printf("  ...\n");
        printf("    nodelist = 0x%p\n", fl->nodelist);
        printf("  ...\n");
        printf("    shapecnt = 0x%p\n", fl->shapecnt);
        printf("    shapesize = 0x%p\n", fl->shapesize);
        printf("    typelist = 0x%p\n", fl->typelist);
        printf("    types = 0x%p\n", fl->types);
        printf("    ntypes = %d\n", fl->ntypes);
        printf("    nodeno = 0x%p\n", fl->nodeno);
        printf("    zoneno = 0x%p\n", fl->zoneno);
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readfacelist: fl = NULL!\n");
    }
    DBFreeFacelist(fl);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

int
test_readzonelist(DBfile * dbfile, long mask)
{
    int     ms;
    DBzonelist *zl = NULL;

    /* Reset the timer. */
    ResetTime();

#ifdef NEW_SILO
    DBSetDataReadMask(mask);
    printf("test_readzonelist: ");
    printMask(stdout, DBGetDataReadMask());
    printf("\n");
#endif
    /* Read the face list and print the struct. */
    zl = DBGetZonelist(dbfile, "zl");
    ms = ElapsedTime();
    if (zl != NULL)
    {
        printf("test_readzonelist: zl = {\n");
        printf("  ...\n");
        printf("    shapecnt = 0x%p\n", zl->shapecnt);
        printf("    shapesize = 0x%p\n", zl->shapesize);
        printf("    shapetype = 0x%p\n", zl->shapetype);
        printf("    nodelist = 0x%p\n", zl->nodelist);
        printf("  ...\n");
        printf("    zoneno = 0x%p\n", zl->zoneno);
        printf("    gzoneno = 0x%p\n", zl->gzoneno);
        printf("};\n");
    } else
    {
        fprintf(stderr, "test_readzonelist: zl = NULL!\n");
    }
    DBFreeZonelist(zl);

    /* Return how many milliseconds since the call to ResetTime. */
    return ms;
}

void
ResetTime(void)
{
    /* Get the start time */
    ftime(&start_time);
}

int
ElapsedTime(void)
{
    int     ms;

    /* Get the end time */
    ftime(&end_time);

    /* Figure out how many milliseconds the rendering took. */
    ms = (int)difftime(end_time.time, start_time.time);
    if (ms == 0)
        ms = end_time.millitm - start_time.millitm;
    else
        ms = ((ms - 1) * 1000) + (1000 - start_time.millitm) +
            end_time.millitm;

    /* Copy the end time into the start time. */
    memcpy((void *)&start_time, (void *)&end_time, sizeof(struct timeb));

    /* Return how many milliseconds it took to render. */
    return ms;
}

void
printTimes(int *ms)
{
    int i = 0;
    printf("Read material (all):   %d ms.\n", ms[i++]);
    printf("Read matnos:           %d ms.\n", ms[i++]);
    printf("Read matlist:          %d ms.\n", ms[i++]);
    printf("Read mixlist:          %d ms.\n", ms[i++]);
    printf("Read material (none):  %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read quadmesh (all):   %d ms.\n", ms[i++]);
    printf("Read quadmesh:         %d ms.\n", ms[i++]);
    printf("Read quadmesh (none):  %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read quadvar (all):    %d ms.\n", ms[i++]);
    printf("Read quadvar:          %d ms.\n", ms[i++]);
    printf("Read quadvar (none):   %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read pointmesh (all):  %d ms.\n", ms[i++]);
    printf("Read pointmesh:        %d ms.\n", ms[i++]);
    printf("Read pointmesh (none): %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read pointvar (all):   %d ms.\n", ms[i++]);
    printf("Read pointvar:         %d ms.\n", ms[i++]);
    printf("Read pointvar (none):  %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read ucdmesh (all):    %d ms.\n", ms[i++]);
    printf("Read ucdmesh coords:   %d ms.\n", ms[i++]);
    printf("Read ucdmesh facelist: %d ms.\n", ms[i++]);
    printf("Read ucdmesh zonelist: %d ms.\n", ms[i++]);
    printf("Read ucdmesh (none):   %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read ucdvar (all):     %d ms.\n", ms[i++]);
    printf("Read ucdvar:           %d ms.\n", ms[i++]);
    printf("Read ucdvar (none):    %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read facelist (all):   %d ms.\n", ms[i++]);
    printf("Read facelist info:    %d ms.\n", ms[i++]);
    printf("Read facelist (none):  %d ms.\n", ms[i++]);
    printf("-----------------------------\n");
    printf("Read zonelist (all):   %d ms.\n", ms[i++]);
    printf("Read zonelist info:    %d ms.\n", ms[i++]);
    printf("Read zonelist (none):  %d ms.\n", ms[i++]);
}

#ifdef NEW_SILO
void
printMask(FILE * fp, long mask)
{
    fprintf(fp, "readMask={");
    if (mask == DBAll)
        fprintf(fp, "DBAll");
    else if (mask == DBNone)
        fprintf(fp, "DBNone");
    else
    {
        if (mask & DBCalc)              fprintf(fp, "DBCalc, ");
        if (mask & DBMatMatnos)         fprintf(fp, "DBMatMatnos, ");
        if (mask & DBMatMatlist)        fprintf(fp, "DBMatMatlist, ");
        if (mask & DBMatMixList)        fprintf(fp, "DBMatMixList, ");
        if (mask & DBCurveArrays)       fprintf(fp, "DBCurveArrays, ");
        if (mask & DBPMCoords)          fprintf(fp, "DBPMCoords, ");
        if (mask & DBPVData)            fprintf(fp, "DBPVData, ");
        if (mask & DBQMCoords)          fprintf(fp, "DBQMCoords, ");
        if (mask & DBQVData)            fprintf(fp, "DBQVData, ");
        if (mask & DBUMCoords)          fprintf(fp, "DBUMCoords, ");
        if (mask & DBUMFacelist)        fprintf(fp, "DBUMFacelist, ");
        if (mask & DBUMZonelist)        fprintf(fp, "DBUMZonelist, ");
        if (mask & DBUVData)            fprintf(fp, "DBUVData, ");
        if (mask & DBFacelistInfo)      fprintf(fp, "DBFacelistInfo, ");
        if (mask & DBZonelistInfo)      fprintf(fp, "DBZonelistInfo, ");
    }
    fprintf(fp, "};");
}
#endif
