## SIPO shift register (for module inputs)
https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf
Can take data in one bit a time serially and output the 8 bits in parallel
#### Operation
1) Keep MR high to prevent resetting
2) Keep OE low to allow output
3) Set STCP low to prevent storage register updating
4) Set SHCP low to initialise the clock
5) Set data bit on DS
6) Set SHCP high to clock data bit in
7) Set SHCP low to finish clock cycle
8) Repeat from step 5 until all 8 data bits have been shifted in
9) Set STCP high to clock data into storage register
10) Set STCP low to prevent storage register updating
11) Data can be shifted out of the device on Q7S to allow daisy chaining

## PISO shift register (for module outputs)
https://assets.nexperia.com/documents/data-sheet/74HC_HCT165.pdf
Can take a 8 bit parallel input and put it into the shift register to send the data serially

#### Operation
1) Keep CE low to enable shifting
2) Set PL high to prevent data immediately entering shift registers
3) Set CP low to initialise the clock
4) Set 8 bits of data on D0-D7.
5) Set PL low to move data into shift registers
6) Set PL high to prevent data changing in the shift registers
7) Set CP high to shift the shift register contents
8) Set CP low to finish clock cycle
9) Repeat from step 7 until all 8 bits have been sent serially
10) Data can be shifted into the device on DS to allow daisy chaining
## Quad 2-input multiplexer (For device discovery identifier)
https://assets.nexperia.com/documents/data-sheet/74HC_HCT157.pdf
Use two of these to switch between device discovery mode and operational mode
## Absolute rotary encoder
https://www.mouser.co.uk/datasheet/3/40/1/ace.pdf
Use part number: EAW0J-B24-AE0128L

- 0-360 degree rotation
- anti-rotation lug position at 9:00
- 128 encoded positions (grey coded)
- Needs a cap
- Constant 8-bit parallel absolute rotation output
## Magnetic connectors
https://www.aliexpress.com/item/1005005284441979.html?spm=a2g0o.productlist.main.2.559812b1IBukg4&aem_p4p_detail=2025111102425519215599861977720001319915&algo_pvid=878ff2ee-8b04-4fe7-b7d3-a95e4ded3db5&algo_exp_id=878ff2ee-8b04-4fe7-b7d3-a95e4ded3db5-1&pdp_ext_f=%7B%22order%22%3A%222796%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21GBP%212.16%210.77%21%21%2119.61%216.97%21%402103835e17628577758241136e5495%2112000046387602811%21sea%21UK%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A231b4884%3Bm03_new_user%3A-29895%3BpisId%3A5000000190273873&curPageLogUid=p8rn6bgITO9j&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005005284441979%7C_p_origin_prod%3A&search_p4p_id=2025111102425519215599861977720001319915_1
## Push buttons
https://www.aliexpress.com/item/1005005791728074.html?spm=a2g0o.productlist.main.22.740biiCWiiCWca&algo_pvid=71d44c9f-18ff-4ae8-baec-8d7d47c7ce61&algo_exp_id=71d44c9f-18ff-4ae8-baec-8d7d47c7ce61-19&pdp_ext_f=%7B%22order%22%3A%22356%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21GBP%214.87%212.30%21%21%2144.32%2120.94%21%4021038df617628580691482946eba50%2112000034367825217%21sea%21UK%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A231b4884%3Bm03_new_user%3A-29895%3BpisId%3A5000000190273874&curPageLogUid=suXBYkpNOk0C&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005005791728074%7C_p_origin_prod%3A#nav-specification
