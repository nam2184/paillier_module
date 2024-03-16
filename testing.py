from random import randint
import libnum
import sys

def mod_exp(base, exponent, modulus):
    result = 1

    base = base % modulus  # Reduce base modulo m

    while exponent > 0:
        if exponent % 2 == 1:
            result = (result * base) % modulus

        base = (base * base) % modulus
        exponent //= 2

    return result

def gcd(a,b):
    """Compute the greatest common divisor of a and b"""
    while b > 0:
        a, b = b, a % b
    return a
    
def lcm(a, b):
    """Compute the lowest common multiple of a and b"""
    return a * b // gcd(a, b)



def L(x,n):
	return ((x-1)//n)

p=11
q=13
m=10




if (len(sys.argv)>1):
	m=int(sys.argv[1])

if (len(sys.argv)>2):
	p=int(sys.argv[2])

if (len(sys.argv)>3):
	q=int(sys.argv[3])

if (p==q):
	print("P and Q cannot be the same")
	sys.exit()

n = p*q

gLambda = lcm(p-1,q-1)

g = 57
if (gcd(g,n*n)==1):
    print(f'g  is: {g}')
else:
    print("WARNING: g is NOT relatively prime to n*n. Will not work!!!")

#r = randint(20,150)
r= 91
l = (pow(g, gLambda, n*n)-1)//n
print(f"l from gMu creation : {l}")
gMu = libnum.invmod(l, n)



k1 = pow(g, m, n*n)
k2 = pow(r, n, n*n)
print(f"k1 is {k1}, k2 is {k2}")

cipher = (k1 * k2) % (n*n)

l = (pow(cipher, gLambda, n*n)-1) // n
print(f"l is :  {l}")
print(f"l is : {l}")
mess= (l * gMu) % n


print("p=",p,"\tq=",q)
print("g=",g,"\tr=",r)
print("================")
print("Mu:\t\t",gMu,"\tgLambda:\t",gLambda)
print("================")
print("Public key (n,g):\t\t",n,g)
print("Private key (lambda,mu):\t",gLambda,gMu)
print("================")
print("Message:\t",mess)

print("Cipher:\t\t",cipher)
print("Decrypted:\t",mess)

print("================")
print("Now we will add a ciphered value of 2 to the encrypted value")


m1=2

k3 = pow(g, m1, n*n)
print("k3 is :" + str(k3))
cipher2 = (k3 * k2) % (n*n)
print("cipher 2 is : " + str(cipher2))
ciphertotal = (cipher* cipher2) % (n*n)

print(ciphertotal)

l = (pow(ciphertotal, gLambda, n*n)-1) // n

mess2= (l * gMu) % n

print("Result:\t\t",mess2)
