import numpy as np
from scipy.optimize import leastsq
from pandas import Series
import math
import matplotlib.pyplot as plt
from scipy import interpolate

#colormoment 9d
acc1=[0.625333,0.62745,0.621493,0.598,0.557967,0.513575,0.473056,0.424807,0.395148,0.365787,0.341395,0.319815,0.300722,0.283156,0.265805,0.254246,0.243239,0.23189,0.223721,0.217037,0.20828,0.201744,0.196509,0.191055,0.186613,0.182252,0.178372,0.178237,0.175159,0.172546,0.169572,0.166372,0.165723,0.163339,0.160574,0.158264,0.155855,0.152706,0.150832,0.149061,0.146977,0.143318,0.141876,0.140378,0.141938,0.140325,0.138968,0.137621,0.133093,0.131901,0.130718,0.131034,0.130013,0.128948,0.127366,0.126423,0.125494,0.12317,0.122378,0.121633]
callback1=[0.00180939,0.00182776,0.0018949,0.00205672,0.00235716,0.00298486,0.00394406,0.0054272,0.00727937,0.0100679,0.0135752,0.0178864,0.0229715,0.0289184,0.0356203,0.0432751,0.0516374,0.0611529,0.0713075,0.0823378,0.0949399,0.107483,0.120771,0.134578,0.149033,0.163902,0.179142,0.189844,0.205376,0.221256,0.23747,0.253868,0.275543,0.292106,0.308921,0.325609,0.341984,0.353081,0.369243,0.385492,0.401154,0.419273,0.434643,0.449584,0.465099,0.47954,0.49395,0.507955,0.521729,0.535426,0.548819,0.565083,0.577942,0.590353,0.599321,0.611165,0.622612,0.636089,0.647316,0.65813]
resultnum1=[1.0094,1.027,1.1238,1.466,2.2066,3.8284,6.4734,10.7474,16.297,24.7162,35.77,50.0834,67.6454,88.4914,113.258,142.16,174.9,211.765,252.306,296.888,345.983,399.059,455.932,515.859,579.41,645.991,715.399,762.34,835.439,911.097,988.897,1068.67,1158.34,1239.46,1322.65,1406.33,1490.2,1578.99,1663.82,1749.3,1833.62,1934.55,2018.7,2101.47,2172.58,2254.1,2334.27,2413.64,2504.04,2581.68,2658.5,2754.24,2828.24,2900.76,2957.35,3027.47,3096.04,3177.77,3243.7,3307.85]

#colorhistogram 24d
acc2=[0.615109,0.59599,0.602434,0.592007,0.582182,0.566695,0.550038,0.520867,0.491164,0.449452,0.423503,0.399638,0.378408,0.35934,0.342204,0.326151,0.310578,0.306146,0.293649,0.284074,0.275473,0.267866,0.259202,0.242499,0.235764,0.232133,0.226127,0.222967,0.212631,0.208555,0.209982,0.206401,0.196187,0.193539,0.191171,0.188478,0.181962,0.180474,0.185766,0.176579,0.174296,0.174958,0.173104,0.176723,0.168819,0.16774,0.165781,0.163854,0.163338,0.167293,0.166107,0.158306,0.160079,0.158791,0.15765,0.15543,0.154404,0.150458,0.154572,0.154199]
callback2=[0.00303189,0.00787742,0.014185,0.0201824,0.0262842,0.0311759,0.0355313,0.0398192,0.0439603,0.047895,0.0525123,0.0575152,0.0633063,0.070066,0.0747951,0.0838523,0.0940245,0.113663,0.126054,0.138756,0.145224,0.159461,0.174473,0.199089,0.215989,0.229225,0.247185,0.264412,0.2784,0.297339,0.318061,0.336112,0.365482,0.383648,0.384796,0.401814,0.425592,0.441394,0.450166,0.464908,0.478957,0.500082,0.513893,0.523666,0.535187,0.548264,0.558232,0.569527,0.581905,0.586564,0.597047,0.614571,0.62247,0.631893,0.643041,0.646135,0.654099,0.677251,0.670409,0.677888]
resultnum2=[3.4886,14.8284,29.4182,45.818,62.3946,78.5016,92.7608,107.316,122.256,134.2,149.88,168.348,189.785,215.349,237.505,272.692,313.359,379.373,431.636,490.028,536.044,608.882,686.949,797.613,887.398,980.03,1076.01,1175.82,1258.09,1364.35,1480.5,1585.81,1724.9,1826.75,1866.81,1969.16,2091.6,2184.71,2247.9,2353.33,2438,2537.99,2620.22,2677.3,2763.49,2842.16,2898.93,2968.85,3049.13,3070.01,3131.76,3224.35,3289.91,3349.17,3407.41,3435.95,3486.9,3617.33,3591.06,3636.93]

#colorhistogram 15d
acc3=[0.6352,0.627125,0.608828,0.596545,0.587182,0.576424,0.565975,0.550559,0.52817,0.499353,0.479633,0.45344,0.431807,0.406284,0.3831,0.366086,0.349782,0.341983,0.324434,0.311799,0.30093,0.291573,0.275667,0.266638,0.259612,0.252386,0.252473,0.245919,0.240406,0.236442,0.230925,0.225114,0.227057,0.223446,0.218805,0.210555,0.207589,0.203465,0.206934,0.20432,0.201764,0.189933,0.188271,0.183076,0.180133,0.177945,0.179164,0.177236,0.180853,0.17915,0.175802,0.174175,0.172471,0.164444,0.163716,0.166887,0.166092,0.163454,0.161865,0.158565]
callback3=[0.0018357,0.00239244,0.0053211,0.0100455,0.0155163,0.0215693,0.0268873,0.0314314,0.0357645,0.0394451,0.0433935,0.0427247,0.0459785,0.0494929,0.0533355,0.0580068,0.0631656,0.071315,0.077661,0.0848836,0.0928347,0.101278,0.113678,0.123871,0.13493,0.146523,0.15666,0.168932,0.181771,0.197207,0.211217,0.22551,0.231775,0.246386,0.261052,0.284182,0.299019,0.314107,0.331522,0.347084,0.362908,0.381961,0.396734,0.407368,0.421671,0.435736,0.449362,0.462917,0.471732,0.48426,0.500133,0.511743,0.523375,0.548157,0.559008,0.563363,0.573912,0.581843,0.591338,0.602979]
resultnum3=[1.0192,1.9228,8.6998,20.6216,35.1706,50.837,65.358,78.7688,92.0948,103.651,116.279,116.786,128.729,141.24,154.805,170.764,189.062,215.309,237.841,264.057,295.177,329.58,376.919,419.411,466.176,517.287,559.659,615.67,674.684,747.063,815.798,888.339,943.447,1019.64,1098.54,1198.69,1281.01,1366.85,1460.03,1549.13,1637.2,1761.3,1849.46,1914.63,1999.81,2085.13,2153.51,2237.37,2304.92,2382.95,2472.93,2546.86,2620.24,2762.57,2831,2858.16,2923.96,2978.46,3041.27,3114.03]

#colorhistogram 9d
acc4=[0.613746,0.595923,0.58118,0.557867,0.52986,0.491751,0.4531,0.422078,0.392483,0.368496,0.346419,0.322652,0.307498,0.298802,0.287651,0.278346,0.269209,0.262361,0.256261,0.254508,0.248824,0.242345,0.236944,0.232026,0.23217,0.227177,0.223349,0.219234,0.215959,0.20636,0.203051,0.199879,0.196435,0.192789,0.190206,0.187774,0.185294,0.182506,0.179362,0.177366,0.174971,0.178062,0.175617,0.17493,0.167237,0.165696,0.164293,0.164524,0.16354,0.161612,0.158245,0.157331,0.156196,0.157851,0.156318,0.155629,0.15666,0.155382,0.153414,0.152689]
callback4=[0.00228945,0.0056114,0.0111253,0.0171058,0.0229603,0.0282885,0.0326798,0.0368597,0.0413696,0.0460098,0.0509975,0.0606246,0.067078,0.0744833,0.0825588,0.0905709,0.0995703,0.10944,0.119913,0.12456,0.136222,0.148211,0.160844,0.17374,0.191668,0.205544,0.219679,0.234324,0.249065,0.261169,0.276417,0.292017,0.307648,0.323185,0.338515,0.354131,0.369774,0.379091,0.394603,0.410079,0.425463,0.448574,0.462564,0.476012,0.498072,0.511692,0.524905,0.516789,0.529664,0.542348,0.562124,0.573532,0.584783,0.597655,0.607664,0.617795,0.622064,0.631488,0.655304,0.664585]
resultnum4=[1.8198,8.9006,22.0746,37.2998,53.0182,67.2074,80.6552,94.4274,109.168,125.136,142.9,173.822,196.955,221.752,249.734,280.033,313.965,352.069,393.13,419.257,465.757,515.498,568.509,624.717,690.924,753.777,819.36,886.672,957.069,1020.37,1096.72,1175.08,1256.72,1342.26,1427.87,1516.33,1605.6,1691.58,1782.27,1873.8,1965.89,2063.05,2152.24,2239.68,2356.26,2444.05,2529.28,2507.5,2590.1,2673.04,2791.2,2868.62,2943.51,3049.14,3118.11,3185.85,3201.75,3265.91,3427.8,3487.88]

#colormoment provided 9d
acc5=[0.642,0.6424,0.6426,0.6436,0.645233,0.639063,0.627115,0.602393,0.565925,0.522198,0.469523,0.431065,0.394517,0.360305,0.330805,0.307173,0.28969,0.273305,0.259002,0.246807,0.244333,0.233739,0.224881,0.217499,0.21147,0.20712,0.202822,0.197535,0.192268,0.188031,0.185242,0.181042,0.177437,0.174099,0.169143,0.166174,0.163203,0.160369,0.158261,0.155699,0.15312,0.152925,0.150654,0.148431,0.145904,0.143849,0.141989,0.139752,0.137947,0.136247,0.134542,0.133046,0.131571,0.128253,0.127029,0.12747,0.126233,0.125019,0.12432,0.1233]
callback5=[0.00182215,0.00182279,0.00182701,0.00185073,0.0018784,0.00192972,0.00201823,0.00221038,0.00257815,0.00309603,0.00380888,0.00507804,0.00676599,0.00905529,0.0119308,0.0156992,0.0202431,0.0255326,0.0318342,0.0391278,0.051633,0.0615361,0.0724852,0.0839442,0.0965883,0.110158,0.120109,0.134655,0.14996,0.165735,0.178615,0.194958,0.21109,0.228069,0.247749,0.265743,0.283374,0.301164,0.322319,0.340262,0.358002,0.369422,0.387096,0.404366,0.431263,0.448605,0.465574,0.475422,0.491663,0.507626,0.524895,0.540771,0.555978,0.568962,0.583489,0.59829,0.612309,0.625844,0.64135,0.65451]
resultnum5=[1.016,1.0164,1.019,1.034,1.0572,1.1344,1.3132,1.6948,2.4386,3.7504,5.744,8.9754,13.6722,20.3472,29.1162,40.757,55.51,73.5492,95.3878,121.117,157.521,193.259,233.567,278.326,327.667,381.304,435.958,497.781,564.008,633.731,689.742,763.972,840.565,920.126,1010.96,1096.74,1183.64,1272.45,1372.08,1463.6,1555.96,1612.32,1704.35,1795.64,1931.5,2024.6,2118.02,2202.07,2293.77,2384.67,2472,2561.43,2649.85,2749.85,2835.37,2901,2984.82,3067.23,3155.94,3235]

def calcf(p,c,alpha):
	return (alpha**2+1)*p*c/((p+c)*alpha**2)

num = len(acc1)
f11=[]
f12=[]
f13=[]
f14=[]
f15=[]
for i in range(0,num):
	f11.append(calcf(acc1[i],callback1[i],1))
	f12.append(calcf(acc2[i],callback2[i],1))
	f13.append(calcf(acc3[i],callback3[i],1))
	f14.append(calcf(acc4[i],callback4[i],1))
	f15.append(calcf(acc5[i],callback5[i],1))

nresultnum1=np.array(resultnum1)
nf11=np.array(f11)
nresultnum2=np.array(resultnum2)
nf12=np.array(f12)
nresultnum3=np.array(resultnum3)
nf13=np.array(f13)
nresultnum4=np.array(resultnum4)
nf14=np.array(f14)
nresultnum5=np.array(resultnum5)
nf15=np.array(f15)

func1=interpolate.interp1d(nresultnum1, nf11, kind='cubic')
func2=interpolate.interp1d(nresultnum2, nf12, kind='cubic')
func3=interpolate.interp1d(nresultnum3, nf13, kind='cubic')
func4=interpolate.interp1d(nresultnum4, nf14, kind='cubic')
func5=interpolate.interp1d(nresultnum5, nf15, kind='cubic')

xnew=np.linspace(5,3000,10000)

plt.plot(xnew,func1(xnew))
plt.plot(xnew,func2(xnew))
plt.plot(xnew,func3(xnew))
plt.plot(xnew,func4(xnew))
plt.plot(xnew,func5(xnew))

plt.xlabel('Retrived Object Number')
plt.ylabel('F-Measure, alpha = 1')

plt.legend(("Color Moment","Color Histogram 24 Dim","Color Histogram 15 Dim","Color Histogram 9 Dim","Provided Color Moment 9 Dim"))
plt.show()