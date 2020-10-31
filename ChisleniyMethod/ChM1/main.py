import math
n=1000000
'''
# integral(-1,1) (|x|)/(sqrt(1-x^2) dx


mas1=[1,2,3,4,5,6,7,8,9,10]
sumfxi=0
for i in mas1:
    xi = math.cos(((2 * i - 1) * math.pi) / (2 * n))
    fxi=math.fabs(xi)
    sumfxi=sumfxi+fxi
    print(i, " ", xi, " ", fxi)
print(sumfxi)
print(sumfxi*(math.pi/n))

'''
'''
# integral (-1,1)  |x|^3/(1-x) dx

m=1
sum=0
while(m<=1000):

    xi=math.cos((math.pi*(2*m-1))/(2*(2*1000+1)))
    xi2=xi**2
    fxi2=math.fabs(xi2)
    ci=(2*math.pi*xi2)/(2*1000+1)
    sum=sum+(fxi2*ci)
    print(m," ",xi2," ",fxi2," ",ci)
    m=m+1
print(sum)
print((3*math.pi)/8)
'''
m=1
sum=0
while(m<=n):
    xi = math.cos(((2 * m - 1) * math.pi) / (2 * n))
    fxi=(math.sin(1000*xi))/(1.0001-xi)
    sum=sum+fxi
    # print(m," ",xi," ",fxi)
    m=m+1
print(sum)
print(sum*(math.pi/n))

