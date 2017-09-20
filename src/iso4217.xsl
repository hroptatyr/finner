<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:s="http://exslt.org/strings"
  version="1.0">
  <!-- generate iso4217.[ch] from
    -  https://www.currency-iso.org/en/home/tables/table-a1.html -->

  <xsl:output method="text"/>
  <xsl:strip-space elements="*"/>

  <xsl:key name="ccy" match="CcyNtry" use="Ccy"/>

  <xsl:template match="CcyTbl">
    <xsl:text>typedef enum {&#0010;</xsl:text>
    <xsl:for-each select="CcyNtry[count(. | key('ccy', Ccy)[1]) = 1 and Ccy]">
      <xsl:sort select="Ccy"/>
      <xsl:text>&#0009;</xsl:text>
      <xsl:value-of select="Ccy"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>
    <xsl:text>} iso4217_t;&#0010;</xsl:text>

    <xsl:text>&#0010;</xsl:text>

    <xsl:text>const struct iso4217_s iso4217[] = {&#0010;</xsl:text>
    <xsl:for-each select="CcyNtry[count(. | key('ccy', Ccy)[1]) = 1 and Ccy]">
      <xsl:sort select="Ccy"/>
      <xsl:text>&#0009;[</xsl:text>
      <xsl:value-of select="Ccy"/>
      <xsl:text>] = {"</xsl:text>
      <xsl:value-of select="Ccy"/>
      <xsl:text>", </xsl:text>
      <xsl:value-of select="s:align(number(CcyNbr), '   ', 'right')"/>
      <xsl:text>, </xsl:text>
      <xsl:choose>
        <xsl:when test="number(CcyMnrUnts) = number(CcyMnrUnts)">
          <xsl:value-of select="CcyMnrUnts"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>-1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>},&#0010;</xsl:text>
    </xsl:for-each>
    <xsl:text>};&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="text()"/>

</xsl:stylesheet>
