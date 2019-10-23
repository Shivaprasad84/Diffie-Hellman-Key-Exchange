#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

typedef unsigned long long int ullint;

bool is_prime(ullint p)
{
    for(ullint i = 2; i <= sqrt(p); i++)
    {
        if(p % i == 0)
            return false;
    }
    return true;
}

ullint gen_prime()
{
    ullint t1 = rand() % 10000;
    ullint t2 = rand() % 10000;
    ullint t3 = t1 * t2;
    while(!is_prime(t3))
    {
        t3++;
    }
    return t3;
}

ullint powmod(ullint a, ullint b, ullint p)
{
    ullint res = 1;
    while(b)
    {
        if(b & 1)
        {
            res = ullint(res * 1ll * a % p), --b;
        }
        else
        {
            a = ullint(a * 1ll * a % p), b >>= 1;
        }
    }
    return res;
}

ullint generator(ullint p)
{
    std::vector<ullint> fact;
    ullint phi = p - 1, n = phi;
    for(ullint i = 2; i <= sqrt(n); ++i)
    {
        if(n % i == 0)
        {
            fact.push_back(i);
            while(n % i == 0)
                n /= i;
        }
    }
    if(n > 1)
    {
        fact.push_back(n);
    }

    for(ullint res = 2; res <= p; ++res)
    {
        bool ok = true;
        for(size_t i = 0; i < fact.size() && ok; ++i)
        {
            ok &= powmod(res, phi/fact[i], p) != 1;
        }
        if(ok) return res;
    }
    return -1;
}



int main()
{
    srand(time(NULL));
    ullint alice_priv_key = 197, bob_priv_key = 123;

    std::cout << "Alice's Private Key: " << alice_priv_key << std::endl; // This is only known to Alice
    std::cout << "Bob's Private Key: " << bob_priv_key << std::endl; // This is only known to Bob

    ullint p = gen_prime();
    ullint g = generator(p);

    std::cout << "p: " << p << std::endl; // This is known globally
    std::cout << "g: " << g << std::endl; // This is known globally

    ullint alice_pub_key = powmod(g, alice_priv_key, p);
    ullint bob_pub_key = powmod(g, bob_priv_key, p);

    std::cout << "Alice's Public Key: " << alice_pub_key << std::endl; // This is sent to Bob
    std::cout << "Bob's Public Key: " << bob_pub_key << std::endl; // This is sent to Alice

    ullint key_generated_at_alice_site = powmod(bob_pub_key, alice_priv_key, p);
    ullint key_generated_at_bob_site = powmod(alice_pub_key, bob_priv_key, p);

    // Key generated at each of the receivers will be same, hence a successful key exchange
    std::cout << "Shared Secret (at Alice): " << key_generated_at_alice_site << std::endl;
    std::cout << "Shared Secret (at Bob): " << key_generated_at_bob_site << std::endl;
}