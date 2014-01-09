------------------+------+--- Original --+-- Presolved --+----------------+----------------+------+---------+--------+-------+--------
Name              | Type | Conss |  Vars | Conss |  Vars |   Dual Bound   |  Primal Bound  | Gap% |  Iters  |  Nodes |  Time |       
------------------+------+-------+-------+-------+-------+----------------+----------------+------+---------+--------+-------+--------
linking               CIP       2       5       1       4                2                2    0.0         0        1     0.0 ok
j301_2                CIP      52      32      36      24               47               47    0.0       173      285     0.0 ok
stein27_super          BP     119     146     126      27                1                1    0.0      5827     1080     0.2 ok
mcf128-4-1            CIP    1832    2596     637    1566               14               14    0.0       897      139     0.1 ok
mcf64-4-1             CIP     928    1308     336     795               10               10    0.0      1013      106     0.1 ok
bell5                 MIP      91     104      35      58       8966406.49       8966406.49    0.0      1432     1273     0.3 ok
blend2                MIP     274     353      94     160         7.598985         7.598985    0.0       818      154     0.2 ok
dcmulti               MBP     290     548     271     547           188182           188182    0.0      3446      161     1.2 ok
egout                 MBP      98     141      37      49         568.1007         568.1007    0.0        67        1     0.0 ok
enigma                 BP      21     100      21     100                0                0    0.0      4380      992     0.2 ok
flugpl                 IP      18      18      13      14          1201500          1201500    0.0       262      202     0.0 ok
gt2                    IP      29     188      28     173            21166            21166    0.0       128        1     0.0 ok
lseu                   BP      28      89      32      63             1120             1120    0.0      2393      494     0.2 ok
MANN_a9.clq            BP      72      45      72      45               16               16    0.0       193        1     0.0 ok
misc03                 BP      96     160      95     138             3360             3360    0.0      2830       71     0.6 ok
p0033                  BP      16      33      13      29             3089             3089    0.0       109        1     0.0 ok
p0548                  BP     176     548      59     101             8691             8691    0.0       943        6     0.1 ok
rgn                   MBP      24     180      24     175       82.1999975       82.1999975    0.0       482        1     0.1 ok
stein27                BP     118      29     118      27               18               18    0.0     15854     4311     0.5 ok
stein27_inf            BP     119      27     118      27            1e+20            1e+20   --        5163     1083     0.2 ok
vpm2                  MBP     402     548     128     181            13.75            13.75    0.0      6356      809     0.8 ok
cross_min.10.10.10     IP     481      91     289     105                1                1    0.0       313        7     0.1 ok
circle                QCP      10       3      10       3       4.57424748       4.57424748    0.0         9        1     0.0 ok
ex1224              MINLP       8      12       9      13     -0.943470556     -0.943470556    0.0       156       16     0.0 ok
ex1266                CIP      96     181      99     109             16.3             16.3    0.0      1554       64     0.3 ok
gastrans            MINLP     144      86      73      55         89.08584         89.08584    0.0       606       25     0.1 ok
m3                  MINLP      43      26      39      22             37.8             37.8    0.0       285       19     0.0 ok
meanvarx            MIQCP      45      36      37      30       14.3692092       14.3692092    0.0      1525      119     0.1 ok
parincQuadratic       QCP       5       3       5       3       49920.5481       49920.5481    0.0        17        1     0.0 ok
tltr                MIQCP      55      49      75      56       48.0666667       48.0666667    0.0       655        7     0.1 ok
4=79-P5=347-P6=307    CIP      13     162     397     534                3                3    0.0     68191    13809     5.8 ok
ormalized-bsg_10_4    CIP      31      20      77      66               -4               -4    0.0       691        6     0.1 ok
ormalized-mds_10_4    CIP      10      10      62      62                2                2    0.0        79        1     0.0 ok
rmalized-mds_50_25    CIP      50      50    1514    1514                3                3    0.0      3628       12     0.9 ok
13queen13.11112183     BP     101     169     114      78            1e+20            1e+20   --        1085        4     0.5 ok
76730--soft-33-100    CIP     102     233     159     295                4                4    0.0      4449      129     1.1 ok
wheel010.lap.opb.p    CIP     273     190     273     189               25               25    0.0      1976       18     1.1 ok
meanvarxsc            CIP      43      36      30      23       14.3692116       14.3692116    0.0      2420      237     0.1 ok
semicon1              CIP       5       5       3       3              1.1              1.1    0.0         2        4     0.0 ok
bart10.shuffled        BP     560     144     560     144               -0               -0    0.0        41        1     0.0 ok
findRoot              CIP       3     101       3     101                1                1    0.0         9        8     0.0 ok
pcu_0.01              CIP     161     609      65     516      0.167527525      0.167527525    0.0     23756     6938     1.0 ok
pcu_0.1               CIP     161     279      65     186      0.185145653      0.185145653    0.0     11614     4123     0.4 ok
sparse2               CIP      64      64      48      64               26               26    0.0     28566    36789     2.1 ok
packorb_1-FullIns     CIP     331      90     319      87               28               28    0.0       579        7     0.2 ok
partorb_1-FullIns     CIP     431     124     419     120                4                4    0.0      1260       13     0.4 ok
------------------+------+-------+-------+-------+-------+----------------+----------------+------+---------+--------+-------+--------

------------------------------[Nodes]---------------[Time]------
  Cnt  Pass  Time  Fail  total(k)     geom.     total     geom.
----------------------------------------------------------------
   46    46     0     0        73      41.9      19.5       1.1 
 shifted geom. [  100/ 10.0]          183.2                 0.4 
----------------------------------------------------------------
@02 timelimit: 3600
@01 SCIP(3.0.2)spx(1.7.2):default [GitHash: 14f3662]
