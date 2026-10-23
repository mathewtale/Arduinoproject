using Arduinoproject.Data;
using Arduinoproject.Models;
using Microsoft.EntityFrameworkCore;

namespace Arduinoproject.Services
{
    public class DbService
    {
        private readonly HttpClient _httpClient;

        public DbService(HttpClient httpClient)
        {
            _httpClient = httpClient;
        }
        public async Task<Quizsave> GetQuizById(int id)
        {
            var result = await _httpClient.GetFromJsonAsync<Quizsave>($"api/Quiz/{id}");
            if(result == null)
                throw new InvalidOperationException("API returned null unexpectedly.");
            return result;
        }
        

        public async Task<List<Quizsave>> GetAllQuizes() 
        {
            var result = await _httpClient.GetFromJsonAsync<List<Quizsave>>("api/Quiz/");

            if(result == null )
                throw new InvalidOperationException("API returned null unexpectedly.");
            return result;
        }
    }
}
