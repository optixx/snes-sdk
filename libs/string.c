char *strdup(const char *s1)
{
    char *s = malloc(strlen(s1) + 1);
    strcpy(s, s1);
    return s;
}
