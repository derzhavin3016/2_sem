#include "patch.h"
  


/**
 * \brief Paatching code in binary file function.
 * \param [in] filename  -name of a file to patch.
 * \return true if all is ok, false otherwise.
 */
bool PatchCode( const char filename[] )
{
  assert(filename != nullptr);

  size_t len = 0;
  unsigned char *buf = FillBuf(filename, &len);

  if (CheckCond(!HashOk(buf, len), "\n!!!ERROR: Different hashes.\n"))
  {
    free(buf);
    return false;
  }

  ////////////////////////////////////////////
  ////////////////////////////////////////////
  /*           Main part of program         */
  buf[Bias]--;
  ////////////////////////////////////////////
  ////////////////////////////////////////////
  ////////////////////////////////////////////

  if (CheckCond(!PutBufToFile(filename, buf, len), "\n!!!ERROR: Error in saving file.\n"))
  {
    free(buf);
    return false;
  }

  free(buf);
  return true;
} /* End of 'PatchCode' function */

/**
 * \brief Check hash of a binary file function
 * \param [in]  buf              - pointer to buffer
 * \param [in]  buf_length       - length of a buffer
 */
bool HashOk( const unsigned char *buf, size_t buf_length )
{
  FILE *conf = fopen(conf_name, "rb");

  if (CheckCond(conf == nullptr, "\n!!!ERROR: configuration file '%s' was not find.\n", conf_name))
    return false;
  
  hash_t check = 0;

  int IsOk = fscanf(conf, "%llu", &check);

  if (CheckCond(IsOk == 0, "\n!!!ERROR: Incorrect config file format.\n") ||
      CheckCond(check != FAQ6Hash(buf, buf_length), "\n!!!ERROR: Incorrect patch file.\n"))
  {
    fclose(conf);
    return false;
  }

  fclose(conf);

  return true;
} /* End of 'HashOk' function */

/**
 * \brief Check error condition and put message to stderr if condition not true.
 * \param [in]  condition - condition to check.
 * \param [in]  err_str   - message to put in stderr.
 * \param [in] ...        - format parameters.
 * \return true if condition is true, false otherwise
 */
bool CheckCond( int condition, const char err_str[], ... )
{
  va_list args;

  if (condition)
  {
    va_start(args, err_str);

    vfprintf(stderr, err_str, args);

    va_end(err_str);
  }

  return (bool)(condition == 1);
} /* End of 'CheckCond' function */


/* END OF 'PATCH.CPP' FILE */