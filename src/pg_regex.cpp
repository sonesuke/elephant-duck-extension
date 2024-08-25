#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <vector>
#include "regalone.h"
#include "regex.h"

#define chr wchar_t
#define re_comp re_wcomp
#define re_exec re_wexec

size_t hexescapes2bin(chr *t, const char *src, size_t mxlen)
{
    const char *s = src;
    const char *xs = src;
    size_t len = 0;
    while ((s = strstr(s, "\\x")) != nullptr)
    {
        int cbin;
        sscanf(&s[2], "%2x", &cbin);
        *const_cast<char*>(s) = '\0';
        len += mbstowcs(&t[len], xs, mxlen - len);
        t[len++] = cbin;
        s += 4;
        xs = s;
    }
    len += mbstowcs(&t[len], xs, mxlen - len);
    return len;
}


int regex_matches(const std::wstring &re, const std::wstring &dat, int nmatch, std::vector<std::wstring> &results)
{
    chr cre[1024 * 4], cdat[1024 * 8];
    size_t crelen, cdatlen;
    regex_t ccre;
    regmatch_t pmatch[100];
    int cflags, rc;
    char buf[1024 * 2];

    crelen = re.size();
    cdatlen = dat.size();
    cflags = REG_ADVANCED | (nmatch ? 0 : REG_NOSUB);
    rc = re_comp(&ccre, cre, crelen, cflags);
    if (rc != REG_OKAY)
    {
        regerror(rc, &ccre, buf, sizeof(buf));
        std::cerr << "Compile error. " << buf << std::endl;
        return 1;
    }
    if (nmatch >= 0 && ccre.re_nsub != nmatch)
    {
        // Additional code here
    }

    rc = re_exec(&ccre, cdat, cdatlen, nullptr, 0, &(pmatch[0]), 0);
    if (rc != REG_OKAY)
    {
        regerror(rc, &ccre, buf, sizeof(buf));
        std::cerr << "Execution error. " << buf << std::endl;
        return 1;
    }

    for (int i = 0; i < nmatch; i++)
    {
        if (pmatch[i].rm_so == -1)
        {
            results.push_back(L"");
        }
        else
        {
            results.push_back(dat.substr(pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so));
        }
    }

    return 0;
}



int regex(int argc, char *argv[])
{
    chr re[1024 * 4], dat[1024 * 8];
    size_t relen, datlen;
    regex_t cre;
    regmatch_t pmatch[100];
    int cflags, nmatch, rc;
    char buf[1024 * 2];

    // memset(&cre, '\0', sizeof(cre));
    nmatch = std::atoi(argv[1]);
    relen = hexescapes2bin(re, argv[2], sizeof(re) / sizeof(chr));
    datlen = hexescapes2bin(dat, argv[3], sizeof(dat) / sizeof(chr));
    cflags = REG_ADVANCED | (nmatch ? 0 : REG_NOSUB);
    rc = re_comp(&cre, re, relen, cflags);
    if (rc != REG_OKAY)
    {
        regerror(rc, &cre, buf, sizeof(buf));
        std::cerr << "Compile error. " << buf << std::endl;
        std::exit(1);
    }
    if (nmatch >= 0 && cre.re_nsub != nmatch)
    {
        // Additional code here
    }

    return 0;
}

